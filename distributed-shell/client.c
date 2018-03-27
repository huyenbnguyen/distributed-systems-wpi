/*
 * Name: Huyen Nguyen
 * Distributed Systems @ WPI (2018)
 *
 */

#include "client.h"

int main(int argc, char **argv) {

	char *default_port = "9898";

	parse_args(argc, argv);
	if (args.port == NULL) {
		args.port = default_port;
	}

	establish_connection();
}

void establish_connection() {
	 int sock, getaddrinfo_ret;

	// look up hostname	
	struct addrinfo hints, *servinfo;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
	hints.ai_socktype = SOCK_STREAM;
	if ((getaddrinfo_ret = getaddrinfo(args.host, args.port, &hints, &servinfo)) != 0) {
		freeaddrinfo(servinfo);
	    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(getaddrinfo_ret));
	    exit(1);
	}

	/* create a TCP socket (an Internet stream socket). */
	puts("Done. Creating socket...");
	if ((sock = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) < 0) {
		freeaddrinfo(servinfo);
		perror("creating socket");
		exit(1);
	}

	/* socket created, so connect to the server */
	puts("Created. Trying connection to server...");
	if (connect(sock, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
		freeaddrinfo(servinfo);
		perror("can't connect");
		exit(1);
	}

	freeaddrinfo(servinfo);

	// /* read from stdin, sending to server, until quit */
	// char buf[80];
	// while (fgets(buf, 80, stdin)) {
	// buf[strlen(buf)-1] = '\0';  remove last \n 
	// printf("sending: '%s'\n", buf);
	// if (write(sock, buf, strlen(buf)) == -1) {
	// perror("write failed");
	// break;
	// }
	// }

	/* close socket */
	close(sock);
}

void parse_args(int argc, char **argv) {
    int c;
    extern int optind, opterr;
    extern char *optarg;

    opterr = 1; /* set to 0 to disable error message */

    while ((c = getopt (argc, argv, "c:s:p:h")) != EOF) {
        switch (c) {
        case 'c':
            args.command = optarg;
            break;
        case 'h':
            print_usage();
            break;
        case 's':
            args.host = optarg; 
            break;
        case 'p':
	        args.port = optarg; 
	        break;  
        default:
            break;
        }
    }
}

/* print a usage message and quit */
void print_usage() {
    fprintf (stderr, "*** Distributed shell client *** \n");
    fprintf (stderr, "usage: ./client  [flags] {-c command}, where flags are:\n");
    fprintf (stderr, "\t{-c command}\tcommand to execute remotely\n");
    fprintf (stderr, "\t{-s host}\thost server is on");
    fprintf (stderr, "\t[-p #]\tport server is on (default is 9898)");
    fprintf (stderr, "\t[-h]\tdisplay this help message\n"); 
    exit(1);
}
