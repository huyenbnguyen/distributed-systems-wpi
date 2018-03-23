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
			// printf("%s\n", "same partition");
			// If the file to be removed is on the same partition as the dumpster directory, 
			// the file should not be copied but instead should be renamed (or hard-linked).
			int rename_ret = rename(file_path, new_name);
			if (rename_ret == -1) {
				free(new_name);	
				perror("Error using rename()");
				return;
			}
			touch_file(new_name, &file_stat);
		}
		free(new_name);	
		// printf("%s\n", "freed");
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
