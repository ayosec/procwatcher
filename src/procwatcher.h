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

#ifndef PROC_WATCHER_H
#define PROC_WATCHER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <proc/readproc.h>

/* fields.c */
int format_proc(const proc_t*, const char*, char*, size_t);
int is_valid_format(const char*);

/* cmdline.c */
#define INVALID_FORMAT -1
struct options {
  pid_t pid;
  unsigned int interval;

  const char* fields;
  FILE* output;
};

void parse_args(int, char**, struct options*);

#endif
