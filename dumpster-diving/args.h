#ifndef ARGS_H
#define ARGS_H

struct arguments {
   int hflag;
   int rflag;
   int fflag;
   char **input_files;
   int num_input_files;
   char *dumpster_path;
} args;

#endif