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
#include <getopt.h>

void parse_args(int argc, char** argv, struct options* options) {
  int option;
  char *errptr;

  memset(options, 0, sizeof(struct options));

  while((option = getopt(argc, argv, "+p:f:i:o:F")) != -1) {
    switch(option) {
      case 'p':
        options->pid = strtoul(optarg, &errptr, 10);
        if(*errptr != '\0') {
          fprintf(stderr, "%s is not a valid PID.\n", optarg);
          exit(1);
        }
        break;

      case 'f':
        if(!template_is_valid(optarg)) {
          fprintf(stderr, "%s is not a valid format string.\n", optarg);
          exit(2);
        }

        options->fields = optarg;
        break;

      case 'i':
        options->interval = strtoul(optarg, &errptr, 10);
        if(*errptr != '\0') {
          fprintf(stderr, "%s is not a valid interval.\n", optarg);
          exit(3);
        }
        break;

      case 'o':
        if(options->output)
          fclose(options->output);

        options->output = fopen(optarg, "w");
        if(!options->output) {
          perror(optarg);
          exit(4);
        }
        break;

      case 'F':
        template_fields_help(stderr);
        exit(0);

      default:
        fprintf(stderr, "%c is not a valid option.\n", option);
        exit(5);
    }
  }

  if(options->fields == 0) {
    fprintf(stderr, "-f not specified\n");
    exit(11);
  }

  /* Set default values */
  if(!options->output)
    options->output = stdout;

  if(options->interval == 0)
    options->interval = 1000;

}
