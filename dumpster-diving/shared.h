#ifndef SHARED_H
#define SHARED_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <libgen.h>
#include <utime.h>

#define MAX_NUM_EXTENSION 9 // maximum number of entensions to be added to differentiate files with the same name

struct arguments {
   int hflag;
   int rflag;
   int fflag;
   char **input_files;
   int num_input_files;
   char *dumpster_path;
} args;

void print_usage();
void parse_args(int argc, char **argv);
void rm_files(dev_t dumpster_device_id);
void remove_file(char *file_path);
void remove_dir(char *dir_path);
char * get_new_name(char *old_name);
void touch_file(char *new_name, struct stat *file_stat);
void copy_file(char *old_name, char *new_name);
void move_and_touch_file(char *old_name, char *new_name, struct stat *file_stat);
void move_and_touch_directory(char *old_name, char *new_name, struct stat *file_stat);
void remove_directory(char *dir_path);
void close_directory(DIR *dir);
void dv_files(dev_t dumpster_device_id);
char *get_cwd();

#endif