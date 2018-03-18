#include "shared.h"

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
			char num[2];
			snprintf(num, 2, "%d", num_extension);
			strcpy(new_name_with_extension, new_name_no_extension);
			snprintf(new_name_with_extension, new_name_no_extension_len+2, "%s.%s", new_name_no_extension, num);
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