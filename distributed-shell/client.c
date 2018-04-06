/*
 * Name: Huyen Nguyen
 * Distributed Systems @ WPI (2018)
 *
 */

#include "shared.h"
#include "client.h"

int main(int argc, char **argv) {
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
	if (!signin_fail(sock)) {
		run_command_on_server(sock);
	}
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
	
	// set timeout to stop reading (useful when print output from server)
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 500000;
	setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

	// display command output from server
	if (strcmp(args.command, "exit") != 0) {
		char buffer[BUFFER_SIZE];
		int bytes_read = 0;
		while (bytes_read >= 0) {
			bzero(buffer,BUFFER_SIZE);
			bytes_read = read(sock_fd, buffer, BUFFER_SIZE-1);
			buffer[bytes_read] = '\0';
			printf("%s", buffer);
		}
	}
	puts("Finished printing command output from server.\n");
}

int signin_fail(int sock_fd) {
	char buffer[BUFFER_SIZE];
	bzero(buffer,BUFFER_SIZE);
	puts("Sending username...");
	int write_ret = write(sock_fd, default_username, strlen(default_username));
	if (write_ret == -1) {
		perror("write() failed");
		return 1;
	}
	puts("Username sent successfully!");

	// read the random string sent by the server
	int bytes_read = read(sock_fd, buffer, BUFFER_SIZE-1);
	if (bytes_read < 0) {
        perror("read() failed");
        return 1;
    }
    buffer[bytes_read] = '\0';
    printf("%s\n", buffer);

    // encrypt password
    char *encrypted_password = crypt(default_password, buffer);
    printf("Client: Encrypted password %s\n", encrypted_password);

    // send encrypted password 
    write_ret = write(sock_fd, encrypted_password, strlen(encrypted_password));
    if (write_ret == -1) {
    	perror("write() failed");
    	return 1;
    }

    // read the response from the server
    bzero(buffer,BUFFER_SIZE);
    bytes_read = read(sock_fd, buffer, BUFFER_SIZE-1);
	if (bytes_read < 0) {
        perror("read() failed");
        return 1;
    }
    buffer[bytes_read] = '\0';
    printf("Server said credentials were %s\n", buffer);

	return strcmp(buffer, "ok");
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
    fprintf (stderr, "\t[-p #]\tport server is on (default is 1024)");
    fprintf (stderr, "\t[-h]\tdisplay this help message\n"); 
    exit(1);
}
