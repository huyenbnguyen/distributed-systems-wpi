/*
 * Name: Huyen Nguyen
 * Distributed Systems @ WPI (2018)
 *
 */

#include "shared.h"

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
	dv_files(dumpster_stat.st_dev);
}

void dv_files(dev_t dumpster_device_id) {
	char *cwd = get_cwd();

	int i;
	for (i = 0; i < args.num_input_files; i++) {
		char *file_path = args.input_files[i];
		struct stat file_stat;
		int file_stat_code = stat(file_path, &file_stat);
		
		// if file doesn't exist, report error
		if (file_stat_code == -1) {
			free(cwd);
			perror("File Path Error in rm_files() ");
			exit(1);
		}

		int is_dir = S_ISDIR(file_stat.st_mode);


		// get new name for file
		char *old_name_copy = (char *) malloc(strlen(file_path)+1); // +1 for null terminator
		if (!old_name_copy) {
			free(cwd);
			fprintf(stderr, "%s\n", "Error: get_new_name() is unable to malloc() for old_name_copy. Aborting...");
			exit(1);
		}
		strcpy(old_name_copy, file_path); // basename() can modify input string => need to copy
		char *base_name = basename(old_name_copy);
		size_t new_name_len = strlen(cwd) + strlen(base_name) + 2; // 1 for terminator, 1 for '/'
		char *new_name = malloc(new_name_len);
		snprintf(new_name, new_name_len, "%s/%s", cwd, base_name);
		printf("%s\n", new_name);
		free(old_name_copy);
		free(cwd);
		
		// check if file already exists
		int access_ret = access(new_name, F_OK);
		if (access_ret == 0) {
			free(new_name);
			fprintf(stderr, "%s\n", "Error: File already exists in current directory. Aborting...");
			exit(1);
		}

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
				free(new_name);
				perror("Error using rename()");
			}
			touch_file(new_name, &file_stat);
		}	
		
		free(new_name);
		printf("%s\n", "freed");
	}
}

char *get_cwd() {
	size_t size = 100;
	while (1) {
		char *buffer = (char *) malloc (size);
		if (getcwd (buffer, size) == buffer)
			return buffer;
		free (buffer);
		size *= 2;
	}
}

void parse_args(int argc, char **argv) {
	int c;
	extern int optind, opterr;
	extern char *optarg;

	opterr = 1; /* set to 0 to disable error message */

	while ((c = getopt (argc, argv, "h")) != EOF) {
		switch (c) {
		case 'h':
			args.hflag++;
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
    fprintf (stderr, "*** dv - move files from the dumpster to current working directory *** \n");
    fprintf (stderr, "usage: ./rm [-h] file1 [file2 ...]\n");
    fprintf (stderr, "\t-h\t\t\tdisplay this help message\n");
    fprintf (stderr, "\tfile1, file2, etc.\tfiles to be moved\n");  
    exit(1);
}