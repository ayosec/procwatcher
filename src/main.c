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

int main(int argc, char** argv) {

  proc_t stat;
  int chars_written;
  char buf[1024];
  useconds_t interval;

  struct options options;
  parse_args(argc, argv, &options);

  interval = options.interval * 1000;

  while(1) {
    if(!get_proc_stats(options.pid, &stat))
      exit(0);

    chars_written = format_proc(&stat, options.fields, buf, sizeof(buf));
    fwrite(buf, chars_written, 1, options.output);
    fputc('\n', options.output);

    usleep(interval);
  }

  return 0;
}
