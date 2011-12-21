
import sys, os

def Fields(target, source, env):
  sys.path.append("src/fields/")

  from fields import builder

  # Find the fields.py in the source.
  program = [x.get_path() for x in source if x.get_path().endswith("/fields.py")][0]
  for output in target:
    builder.build(open(output.get_path(), "w"))

  return None

env = Environment(CFLAGS="-Wall -D_GNU_SOURCE=1",
                  CC=os.getenv("CC", "gcc"),
                  BUILDERS = { 'Fields': Builder(action = Fields) })

if os.getenv("DEBUG"):
  env.Append(CFLAGS = " -g")

env.ParseConfig('pkg-config --cflags --libs libproc-ng')

env.Fields('src/fields.c', Glob('src/fields/*'))
env.Program('procwatcher', Glob('src/*.c'))

# vim: syntax=python
