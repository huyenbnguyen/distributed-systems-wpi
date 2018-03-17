/*
 * Name: Huyen Nguyen
 * Distributed Systems @ WPI (2018)
 *
 */

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
#include "args.h"

#define MAX_NUM_EXTENSION 9 // maximum number of entensions to be added to differentiate files with the same name

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
void close_and_remove_directory(DIR *dir, char* dir_path);

int main(int argc, char **argv) {
	static const char *ENV_NAME = "DUMPSTER"; // environment variable

	// parse command line arguments
	parse_args(argc, argv);

	// check if environment variable (aka DUMPSTER) has been set
	args.dumpster_path = getenv(ENV_NAME);
	if (!args.dumpster_path) {
		fprintf(stderr, "%s\n", "Error: DUMPSTER hasn't been set. Aborting...");
		exit(1);
	}
	struct stat dumpster_stat;
	int dumpster_stat_code = stat(args.dumpster_path, &dumpster_stat);

	// if dumpster path is invalid, report error
	if (dumpster_stat_code == -1) {
		perror("Dumpster Path Error ");
		exit(1);
	}

	// main logic
	rm_files(dumpster_stat.st_dev);
}

void rm_files(dev_t dumpster_device_id) {
	int i;
	for (i = 0; i < args.num_input_files; i++) {
		char *file_path = args.input_files[i];
		struct stat file_stat;
		int file_stat_code = stat(file_path, &file_stat);
		
		// if file doesn't exist, report error
		if (file_stat_code == -1) {
			perror("File Path Error in rm_files() ");
			exit(1);
		}

		int is_dir = S_ISDIR(file_stat.st_mode);

		// if file is directory, check if -r is specified
		if (is_dir) {
			if (!args.rflag) {
				fprintf(stderr, "%s\n", "Error: -r is missing for a directory. Aborting...");
				exit(1);
			}
		}

		// force remove
		if (args.fflag) {
			if (is_dir) {
				remove_dir(file_path);
			} else {
				remove_file(file_path);
			}
			exit(0);
		}

		// don't force remove, use dumpster
		char *new_name = get_new_name(file_path);

		// If the file being removed is not on the same partition as the dumpster directory, 
		// your rm must copy the file and then delete it (via the unlink() or remove() system call).
		if (file_stat.st_dev != dumpster_device_id) {
			if (is_dir) {
				move_and_touch_directory(file_path, new_name, &file_stat);
			} else {
				move_and_touch_file(file_path, new_name, &file_stat);
			}
		} else { 
			printf("%s\n", "same partition");
			// If the file to be removed is on the same partition as the dumpster directory, 
			// the file should not be copied but instead should be renamed (or hard-linked).
			int rename_ret = rename(file_path, new_name);
			if (rename_ret == -1) {
				perror("Error using rename()");
			}
			touch_file(new_name, &file_stat);
		}
		free(new_name);	
		printf("%s\n", "freed");
	}
}

void move_and_touch_file(char *old_name, char *new_name, struct stat *file_stat) {
	copy_file(old_name, new_name);
	int remove_ret = remove(old_name);
	if (remove_ret == -1) {
		perror("Error removing old file");
		return;
	}
	touch_file(new_name, file_stat);
}

void close_and_remove_directory(DIR *dir, char* dir_path) {
	int closedir_ret = closedir(dir);
	if (closedir_ret == -1) {
		perror("Error closing directory");
		return;
	}
	int rmdir_ret = rmdir(dir_path); // remove empty directory
	if (rmdir_ret == -1) {
		perror("Error removing directory");
		return;
	}
}

void move_and_touch_directory(char *old_name, char *new_name, struct stat *file_stat) {
	DIR *dir = opendir(old_name);
	if (!dir) {
		perror("Cannot Open Directory Error in move_and_touch_directory()");
		return;
	}
	
	if (mkdir(new_name, 0700) == -1) {
		perror("Error mkdir() in move_and_touch_directory() ");
		return;
	}

	touch_file(new_name, file_stat);
	struct dirent *dirent_struct = readdir(dir);
	while (dirent_struct) {
		char *file_to_move = dirent_struct->d_name;

		// skip names '.' and '..'
		if ((strcmp(file_to_move, ".") != 0 && strcmp(file_to_move, "..") != 0)) {
			// get full path of the file_to_delete
			size_t full_path_new_len = strlen(new_name) + strlen(file_to_move) + 2; // 1 for terminator, 1 for '/' (doesn't matter whether dir_path ends with '/', tested)
			size_t full_path_old_len = strlen(old_name) + strlen(file_to_move) + 2;
			char *full_path_new = malloc(full_path_new_len);
			char *full_path_old = malloc(full_path_old_len);
				
			if (full_path_new && full_path_old) {
				snprintf(full_path_new, full_path_new_len, "%s/%s", new_name, file_to_move);
				snprintf(full_path_old, full_path_old_len, "%s/%s", old_name, file_to_move);
				struct stat file_stat_current;
				int file_stat_code = stat(full_path_old, &file_stat_current);
			
				// if file doesn't exist, report error
				if (file_stat_code == -1) {
					perror("File Path Error in remove_dir() ");
					return;
				}

				if (S_ISDIR(file_stat_current.st_mode)) {
					move_and_touch_directory(full_path_old, full_path_new, &file_stat_current);
				} else {
					move_and_touch_file(full_path_old, full_path_new, &file_stat_current);
				}
				free(full_path_new);
				free(full_path_old);
			} else {
				if (full_path_new) free(full_path_new);
				else free(full_path_old);
				fprintf(stderr, "%s\n", "Error: remove_dir() is unable to malloc(). Aborting...");
				return;
			}
		}
		dirent_struct = readdir(dir);
	}
	close_and_remove_directory(dir, old_name);
}

void copy_file(char *old_name, char *new_name) {
	FILE *old_file_ptr, *new_file_ptr;
	old_file_ptr = fopen(old_name, "r");
	new_file_ptr = fopen(new_name, "w");
	if (old_file_ptr == NULL || new_file_ptr == NULL) {
		perror("Error opening file in copy_file()");
		return;
	}
	char c = fgetc(old_file_ptr);
	while (c != EOF) {
		fputc(c, new_file_ptr);
		c = fgetc(old_file_ptr);
	}
	fclose(old_file_ptr);
	fclose(new_file_ptr);
}

void touch_file(char *new_name, struct stat *file_stat) {

	struct utimbuf puttime;	/* to set time */
	puttime.actime = file_stat->st_atime;
	puttime.modtime = file_stat->st_mtime;
	int utime_ret = utime(new_name, &puttime);
	int chmod_ret = chmod(new_name, file_stat->st_mode);
	int chown_ret = chown(new_name, file_stat->st_uid, file_stat->st_gid);

	if (utime_ret == -1 || chmod_ret == -1 || chown_ret == 1) { 
		perror("Error in touch_files() ");
		return;
	}
}

char *get_new_name(char *old_name) {
	short num_extension = 0;
	char *old_name_copy = (char *) malloc(strlen(old_name)+1); // +1 for null terminator
	if (!old_name_copy) {
		fprintf(stderr, "%s\n", "Error: get_new_name() is unable to malloc() for old_name_copy. Aborting...");
		exit(1);
	}
	strcpy(old_name_copy, old_name); // basename() can modify input string => need to copy
	char *base_name = basename(old_name_copy);
	size_t new_name_no_extension_len = strlen(args.dumpster_path) + strlen(base_name) + 2; // 1 for terminator, 1 for '/'
	char *new_name_no_extension = malloc(new_name_no_extension_len);
	if (new_name_no_extension) {
		snprintf(new_name_no_extension, new_name_no_extension_len, "%s/%s", args.dumpster_path, base_name); 
		free(old_name_copy);
		char *new_name_with_extension = (char *) malloc(new_name_no_extension_len+2); // 1 for '.', 1 for number
		if (!new_name_with_extension) {
			free(new_name_no_extension);
			fprintf(stderr, "%s\n", "Error: get_new_name() is unable to malloc() for old_name_copy. Aborting...");
			exit(1);
		}
		strcpy(new_name_with_extension, new_name_no_extension);
		// find number extension
		while (access(new_name_with_extension, F_OK) == 0 && num_extension <= MAX_NUM_EXTENSION + 1) {
			num_extension++;
			char num = num_extension + '0'; // convert number to corresponding character
			char *num_ptr = &num;
			strcpy(new_name_with_extension, new_name_no_extension);
			snprintf(new_name_with_extension, new_name_no_extension_len+2, "%s.%s", new_name_with_extension, num_ptr);
		}
		if (num_extension > MAX_NUM_EXTENSION) {
			free(new_name_no_extension);
			free(new_name_with_extension);
			fprintf(stderr, "%s\n", "Error: Dumpster is full. Aborting...");
			exit(1);
		} else if (num_extension > 0) { // if an number extension is needed 
			free(new_name_no_extension);
			return new_name_with_extension;
		}
		free(new_name_with_extension);
		return new_name_no_extension;	
	} 
	free(old_name_copy);
	fprintf(stderr, "%s\n", "Error: get_new_name() is unable to malloc() for new_name_no_extension. Aborting...");
	exit(1);
}

void remove_dir(char *dir_path) {
	DIR *dir = opendir(dir_path);
	if (!dir) {
		perror("Cannot Open Directory Error");
		exit(1);
	}

	struct dirent *dirent_struct = readdir(dir);
	while (dirent_struct) {
		char *file_to_delete = dirent_struct->d_name;

		// skip names '.' and '..'
		if ((strcmp(file_to_delete, ".") != 0 && strcmp(file_to_delete, "..") != 0)) {
			// get full path of the file_to_delete
			size_t full_path_len = strlen(dir_path) + strlen(file_to_delete) + 2; // 1 for terminator, 1 for '/' (doesn't matter whether dir_path ends with '/', tested)
			char *full_path = malloc(full_path_len);
				
			if (full_path) {
				snprintf(full_path, full_path_len, "%s/%s", dir_path, file_to_delete);
				struct stat file_stat;
				int file_stat_code = stat(full_path, &file_stat);
			
				// if file doesn't exist, report error
				if (file_stat_code == -1) {
					perror("File Path Error in remove_dir() ");
					return;
				}

				if (S_ISDIR(file_stat.st_mode)) {
					remove_dir(full_path);
				} else {
					remove_file(full_path);
				}
				free(full_path);
			} else {
				fprintf(stderr, "%s\n", "Error: remove_dir() is unable to malloc(). Aborting...");
				return;
			}
		}
		dirent_struct = readdir(dir);
	}
	close_and_remove_directory(dir, dir_path);
}

void remove_file(char *file_path) {
	int ret = remove(file_path);
	if (ret == -1) {
		perror("Remove File Error in remove_file() ");
		exit(1);
	}
}

void parse_args(int argc, char **argv) {
	int c;
	extern int optind, opterr;
	extern char *optarg;

	opterr = 1; /* set to 0 to disable error message */

	while ((c = getopt (argc, argv, "fhr")) != EOF) {
		switch (c) {
		case 'f':
			args.fflag++;
			break;
		case 'h':
			args.hflag++;
			break;
		case 'r':
		  	args.rflag++;
		  	break;
		default:
			break;
		}
	}
	// printf("%s",*(argv+1));
	args.num_input_files = argc - optind;
	args.input_files = argv + optind;

	// -h flag 
	if (args.hflag) print_usage();

	// check if there's input file
	if (args.num_input_files == 0) {
		fprintf(stderr, "%s\n", "Error: No input file specified. Aborting...");
		exit(1);
	}
}

/* print a usage message and quit */
void print_usage(void) {
    fprintf (stderr, "*** rm - move files to the dumpster *** \n");
    fprintf (stderr, "usage: ./rm [-f] [-h] [-r] file1 [file2 ...]\n");
    fprintf (stderr, "\t-f\t\t\tforce a complete remove, do not move to dumpster\n");
    fprintf (stderr, "\t-h\t\t\tdisplay this help message\n");
    fprintf (stderr, "\t-r\t\t\tcopy directories recursively\n"); 
    fprintf (stderr, "\tfile1, file2, etc.\tfiles to be removed\n");  
    exit(1);
}


