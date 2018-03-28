/*
 * Name: Huyen Nguyen
 * Distributed Systems @ WPI (2018)
 *
 */

#include "client.h"

int main(int argc, char **argv) {
	initialize_default_values();
	parse_args(argc, argv);
	if (args.port == NULL) {
		args.port = DEFAULT_PORT;
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
		perror("socket() failed");
		exit(1);
	}

	/* socket created, so connect to the server */
	puts("Created. Trying connection to server...");
	if (connect(sock, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
		freeaddrinfo(servinfo);
		perror("connect() failed");
		exit(1);
	}

	freeaddrinfo(servinfo);

	int sign_in_successful = signin(sock);
	if (sign_in_successful) {

		run_command_on_server(sock);
	}

	

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

void run_command_on_server(int sock_fd) {
	if (args.command == NULL || args.host == NULL) {
		puts("Command or host was not specified. Aborting...");
		print_usage();
	}

	printf("Command is %s\n", args.command);

	// send command to server
	int write_ret = write(sock_fd, args.command, strlen(args.command));
	if (write_ret == -1) {
		perror("write() failed");
		return;
	}
	puts("Command sent successfully!");

	

}

int signin(int sock_fd) {
	char buffer[BUFFER_SIZE];
    bzero(buffer,BUFFER_SIZE);
	puts("Sending username...");
	int write_ret = write(sock_fd, DEFAULT_USERNAME, strlen(DEFAULT_USERNAME));
	if (write_ret == -1) {
		perror("write() failed");
		return 0;
	}
	puts("Username sent successfully!");

	// read the random string sent by the server
	int bytes_read = read(sock_fd, buffer, BUFFER_SIZE-1);
	if (bytes_read < 0) {
        perror("read() failed");
        return 0;
    }
    buffer[bytes_read] = '\0';
    printf("%s\n", buffer);

    // encrypt password
    char *encrypted_password = crypt(DEFAULT_PASSWORD, buffer);
    printf("Client: Encrypted password %s\n", encrypted_password);

    // send encrypted password 
    write_ret = write(sock_fd, encrypted_password, strlen(encrypted_password));
    if (write_ret == -1) {
    	perror("write() failed");
    	return 0;
    }

    // read the response from the server
    bzero(buffer,BUFFER_SIZE);
    bytes_read = read(sock_fd, buffer, BUFFER_SIZE-1);
	if (bytes_read < 0) {
        perror("read() failed");
        return 0;
    }
    buffer[bytes_read] = '\0';
    printf("Server said credentials were %s\n", buffer);

	return strcmp(buffer, "ok") == 0;
}

void initialize_default_values() {
	
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
