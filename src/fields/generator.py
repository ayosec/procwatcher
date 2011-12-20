
import os.path
from sets import Set
from string import Template
from textwrap import TextWrapper

def tree_fields_cmp(names, offset):
  items = []
  for letter in Set([i[0] for i in names if len(i) > 0]):
    subnames = Set([i[1:] for i in names if len(i) > 0 and i[0] == letter])
    items.append("(name[%d] == '%s' && (%s))" % (offset, letter, tree_fields_cmp(subnames, offset + 1)))

  if(len([i for i in names if len(i) == 0]) > 0):
    items.append("name[%d] == '\\0'" % offset)

  return " || ".join(items)

class Builder:
  def __init__(self):
    self.groups = []
    self.current_group = None

  def group(self, name):
    self.current_group = {"name": name, "fields": []}
    self.groups.append(self.current_group)

  def field(self, name, format_key, desc):
    self.current_group["fields"].append({
      "type": "field",
      "name": name,
      "format": format_key,
      "desc": desc
    })

  def generate_help(self):
    wrapper = TextWrapper(subsequent_indent = " " * 15)

    result = []
    for group in self.groups:
      result.append("\n%s:\n" % group["name"])
      for field in group["fields"]:
        result.append("%12s - %s" % (field["name"], "\n".join(wrapper.wrap(field["desc"]))))

    result.append("\n")

    return('\n'.join(result).
      replace('"', '\\"').
      replace("'", "\\'").
      replace("\n", "\\n"))

  def build(self, output):
    template = Template(open(os.path.dirname(__file__) + "/fields.c.tmpl").read())
    attributes = {}

    fields = reduce(lambda a, b: a + b, [g["fields"] for g in self.groups])

    # Used in template_is_valid to check if the field item is valid
    #attributes["name_comparator"] = "(" + " || ".join(
    #  [("""strcmp("%s", name) == 0""" % f["name"]) for f in fields]
    #) + ")"
    attributes["name_comparator"] = tree_fields_cmp([f["name"] for f in fields], 0)

    # Blocks to generate the values
    to_generate = []
    to_call = []
    for field in fields:
      fn_name = "_fv_" + field["name"]
      if field["type"] == "field":

        to_call.append("""
      if(strcmp(name, "%(name)s") == 0) {
        written_chars += snprintf(output_buffer + written_chars,
                                  output_size - written_chars,
                                  "%(format)s",
                                  proc->%(name)s);
        goto skip_cmps;
      }""" % field)

    attributes["values"] = "\n" + "\n\n".join(to_call)

    attributes["help_fields"] = self.generate_help()

    attributes["mark_for_generated"] = "GENERATED FILE. DO NOT EDIT"

    # Static header
    output.write(template.substitute(attributes))
