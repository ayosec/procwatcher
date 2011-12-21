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
#include <sys/wait.h>


void sigchld_handler(int dummy) {
  wait(&dummy);
}

int main(int argc, char** argv) {

  proc_t stat;
  int chars_written;
  char stat_line[1024];
  char proc_path[64];
  struct options options;

  parse_args(argc, argv, &options);

  /* If any argument is present, execute it */
  if(options.command) {
    pid_t pid = fork();

    if(pid == -1) { /* failed fork */
      perror("fork");
      exit(12);
    }

    if(pid == 0) { /* child */
      execvp(options.command[0], (char * const*)options.command);

      /* If this line is reached the command could not be started. */
      perror(options.command[0]);
      kill(getppid(), SIGINT);
      exit(-1);
    }

    signal(SIGCHLD, sigchld_handler);
    options.pid = pid;
  }

  snprintf(proc_path, sizeof(proc_path), "/proc/%d/stat", options.pid);
  options.interval *= 1000;

  while(1) {
    /* Check if the process is still running */
    if(access(proc_path, R_OK) == -1)
      exit(0);

    if(!get_proc_stats(options.pid, &stat))
      exit(0);

    chars_written = template_format(&stat, options.fields, stat_line, sizeof(stat_line));
    fwrite(stat_line, chars_written, 1, options.output);
    fputc('\n', options.output);

    usleep(options.interval);
  }

  return 0;
}
