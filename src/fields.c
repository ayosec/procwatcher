/*
 * Copyright (C) 2011 Ayose <http://ayosec.com/>
 *
 * This file is part of ProcWatcher.
 *
 * ProcWatcher is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ProcWatcher is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "procwatcher.h"

int is_valid_format(const char* format) {

  proc_t stat;
  char buf[1024];

  if(!get_proc_stats(getpid(), &stat)) {
    perror("get_proc_stats");
    exit(10);
  }

  return format_proc(&stat, format, buf, sizeof(buf)) != INVALID_FORMAT;
}

int format_proc(const proc_t* proc, const char* source_fields, char* buffer, size_t buffer_size) {

  char *current_field, *next_field;
  int buffer_offset;

  memset(buffer, 0, buffer_size);
  buffer_offset = 0;

  next_field = strdupa(source_fields);

start:

  /* Extract the first item to show and save the next one in next_field */
  current_field = next_field;
  if(current_field[0] == '\0' || buffer_offset >= buffer_size)
    return buffer_offset;

  next_field = strchrnul(current_field, ',');
  if(next_field[0] != '\0') {
    *next_field = '\0';
    ++next_field;
  }

  /* Helpers to produce the output */

#define append(format, value) \
  do { \
    int chars_written = snprintf(buffer + buffer_offset, buffer_size - buffer_offset, format, value); \
    if(chars_written < 0) \
      return -1; \
    buffer_offset += chars_written; \
    if(buffer_offset < buffer_size && next_field[0] != '\0') { \
      buffer[buffer_offset++] = ','; \
    } \
    goto start; \
  } while(0)


#define field(name, format) \
  do { \
    if(strcmp(current_field, #name) == 0) \
      append(format, proc->name); \
  } while(0)


#define  fmt_int                 "%d"
#define  fmt_unsigned            "%u"
#define  fmt_char                "%c"
#define  fmt_unsigned_long_long  "%llu"
#define  fmt_long                "%ld"
#define  fmt_unsigned_long       "%lu"

  /* Fields explained in /usr/include/proc/readproc.h */

  field(tid, fmt_int);
  field(ppid, fmt_int);
  field(pcpu, fmt_unsigned);
  field(state, fmt_char);
  field(utime, fmt_unsigned_long_long);
  field(stime, fmt_unsigned_long_long);
  field(cutime, fmt_unsigned_long_long);
  field(cstime, fmt_unsigned_long_long);
  field(start_time, fmt_unsigned_long_long);
  field(priority, fmt_long);
  field(nice, fmt_long);
  field(rss, fmt_long);
  field(alarm, fmt_long);
  field(size, fmt_long);
  field(resident, fmt_long);
  field(share, fmt_long);
  field(trs, fmt_long);
  field(lrs, fmt_long);
  field(drs, fmt_long);
  field(dt, fmt_long);
  field(vm_size, fmt_unsigned_long);
  field(vm_lock, fmt_unsigned_long);
  field(vm_rss, fmt_unsigned_long);
  field(vm_data, fmt_unsigned_long);
  field(vm_stack, fmt_unsigned_long);
  field(vm_swap, fmt_unsigned_long);
  field(vm_exe, fmt_unsigned_long);
  field(vm_lib, fmt_unsigned_long);
  field(rtprio, fmt_unsigned_long);
  field(sched, fmt_unsigned_long);
  field(vsize, fmt_unsigned_long);
  field(rss_rlim, fmt_unsigned_long);
  field(flags, fmt_unsigned_long);
  field(min_flt, fmt_unsigned_long);
  field(maj_flt, fmt_unsigned_long);
  field(cmin_flt, fmt_unsigned_long);
  field(cmaj_flt, fmt_unsigned_long);
  field(pgrp, fmt_int);
  field(session, fmt_int);
  field(nlwp, fmt_int);
  field(tgid, fmt_int);
  field(tty, fmt_int);
  field(euid, fmt_int);
  field(egid, fmt_int);
  field(ruid, fmt_int);
  field(rgid, fmt_int);
  field(suid, fmt_int);
  field(sgid, fmt_int);
  field(fuid, fmt_int);
  field(fgid, fmt_int);
  field(tpgid, fmt_int);
  field(exit_signal, fmt_int);
  field(processor, fmt_int);

#ifdef OOMEM_ENABLE
  field(processor, oom_score);
  field(processor, oom_adj);
#endif

  /* If no field() item is used we can assume that the string is not valid */
  return INVALID_FORMAT;

}
