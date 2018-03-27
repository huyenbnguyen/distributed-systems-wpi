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

    printf("%s\n", "./server activating");
    printf("\tport:%s\n", args.port);
    printf("\tdirectory:%s\n", args.current_directory);

    establish_connection();

    free(current_directory);
}

void establish_connection() {
    int sock, serv_host_port, newsock, getaddrinfo_ret;
    socklen_t clilen;
    struct sockaddr_in cli_addr, serv_addr;
    
    // see here for the reason: https://beej.us/guide/bgnet/html/multi/getaddrinfoman.html 
    struct addrinfo hints, *servinfo;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // allow hostname to be NULL
    if ((getaddrinfo_ret = getaddrinfo(NULL, args.port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(getaddrinfo_ret));
        exit(1);
    }  

    /* create socket from which to read */
    if ((sock = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) < 0) {
        perror("creating socket");
        return;
    }

    /* bind our local address so client can connect to us */
    if (bind(sock, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
        perror("can't bind to local address");
        return;
    }

    /* mark socket as passive, with backlog num */
    if (listen(sock, QUEUE_LIMIT) == -1) {
        perror("listen");
        return;
    }

    printf("Socket ready to go! Accepting connections....\n\n");
    clilen = sizeof(cli_addr);

    /* wait here (block) for connection */ 
    newsock = accept(sock, (struct sockaddr *) &cli_addr, &clilen);
    if (newsock < 0) {
        perror("accepting connection");
        return;
    }
   
   freeaddrinfo(servinfo);
   //  char message[1024];
   // int bytes;
   // /* read data until no more */
   // while ((bytes = read(newsock, message, 1024)) > 0) {
   //   message[bytes] = '\0';  do this just so we can print as string 
   //   printf("received: '%s'\n", message);
   // }

   // if (bytes == -1)
   //   perror("error in read");
   // else
   //   printf("server exiting\n");

    /* close connected socket and original socket */
    close(newsock);
    close(sock);
    
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