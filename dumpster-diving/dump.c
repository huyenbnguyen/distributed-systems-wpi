/*
 * Name: Huyen Nguyen
 * Distributed Systems @ WPI (2018)
 *
 */

#include "shared.h"

int main(int argc, char **argv) {
	static const char *ENV_NAME = "DUMPSTER"; // environment variable

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

	// main logicm
	int is_dir = S_ISDIR(dumpster_stat.st_mode);
	if (is_dir) {
		remove_dir(args.dumpster_path); // this function also deletes the DUMPSTER directory itself
		if (mkdir(args.dumpster_path, 0700) == -1) { // recreate the DUMPSTER directory
			perror("Error making DUMPSTER directory ");
			exit(1);
		}
	} else {
		fprintf(stderr, "%s\n", "Error: DUMPSTER is not a directory. Aborting...");
		exit(1);
	}

}

/* print a usage message and quit */
void print_usage(void) {
    fprintf (stderr, "*** dump - delete files in the dumpster permanently*** \n");
    fprintf (stderr, "usage: ./dump \n"); 
    exit(1);
}