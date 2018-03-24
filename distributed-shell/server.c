/*
 * Name: Huyen Nguyen
 * Distributed Systems @ WPI (2018)
 *
 */

#include "server.h"

int main(int argc, char **argv) {
    char *default_port = "9898";
    char *current_directory = get_cwd();

    parse_args(argc, argv, current_directory);
    if(args.port == NULL) {
        args.port = default_port;
    }
    if (args.current_directory == NULL) {
        args.current_directory = current_directory;
    } else {
        int chdir_ret = chdir(args.current_directory);
        if (chdir_ret == -1) {
            free(current_directory);
            perror("chdir() error");
            exit(1);
        }
    }

    free(current_directory);
}

/* print a usage message and quit */
void print_usage(char *current_directory) {
    fprintf (stderr, "*** Distributed shell server *** \n");
    fprintf (stderr, "usage: ./server [flags], where flags are\n");
    fprintf (stderr, "\t-p #\tport to serve on (default in 9898)\n");
    fprintf (stderr, "\t-d dir\tdirectory to serve out of (default is %s)\n", current_directory);
    fprintf (stderr, "\t-h\tdisplay this help message\n"); 
    free(current_directory);
    exit(1);
}

void parse_args(int argc, char **argv, char *current_directory) {
    int c;
    extern int optind, opterr;
    extern char *optarg;

    opterr = 1; /* set to 0 to disable error message */

    while ((c = getopt (argc, argv, "p:hd:")) != EOF) {
        switch (c) {
        case 'p':
            args.port = optarg;
            break;
        case 'h':
            print_usage(current_directory);
            break;
        case 'd':
            args.current_directory = optarg; 
            break;
        default:
            break;
        }
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