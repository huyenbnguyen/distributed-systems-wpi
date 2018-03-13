/*
 * Name: Huyen Nguyen
 * Distributed Systems @ WPI (2018)
 *
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "args.h"


void print_usage();

void parse_args(int argc, char **argv);


int main(int argc, char **argv) {
	parse_args(argc, argv);
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
	args.num_input_files = argc - optind;
	args.input_files = argv + optind;
	int errflag = (args.fflag > 1) || (args.hflag > 1) || (args.rflag > 1); // cannot provide more than 1 instance of optional arguments
	if (errflag || args.hflag) print_usage();
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


