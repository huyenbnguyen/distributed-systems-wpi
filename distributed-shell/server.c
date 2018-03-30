/*
 * Name: Huyen Nguyen
 * Distributed Systems @ WPI (2018)
 *
 */

#include "server.h"

int main(int argc, char **argv) {
    char *current_directory = get_cwd();

    parse_args(argc, argv, current_directory);
    if(args.port == NULL) {
        args.port = DEFAULT_PORT;
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
    int server_sock_fd, serv_host_port, incoming_sock_fd, getaddrinfo_ret;
    socklen_t clilen;
    struct sockaddr_in cli_addr, serv_addr;
    
    // see here for the reason: https://beej.us/guide/bgnet/html/multi/getaddrinfoman.html 
    struct addrinfo hints, *servinfo;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // allow hostname to be NULL
    if ((getaddrinfo_ret = getaddrinfo(NULL, args.port, &hints, &servinfo)) != 0) {
        freeaddrinfo(servinfo);
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(getaddrinfo_ret));
        return;
    }  

    /* create socket from which to read */
    if ((server_sock_fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) < 0) {
        freeaddrinfo(servinfo);
        perror("creating socket");
        return;
    }

    /* bind our local address so client can connect to us */
    if (bind(server_sock_fd, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
        freeaddrinfo(servinfo);
        perror("can't bind to local address");
        return;
    }

    freeaddrinfo(servinfo);

    /* mark socket as passive, with backlog num */
    if (listen(server_sock_fd, QUEUE_LIMIT) == -1) {
        
        perror("listen");
        return;
    }

    printf("Socket ready to go! Accepting connections....\n\n");
    clilen = sizeof(cli_addr);

    // while (1) {
        /* wait here (block) for connection */ 
        incoming_sock_fd = accept(server_sock_fd, (struct sockaddr *) &cli_addr, &clilen);
        if (incoming_sock_fd < 0) {
            perror("accepting connection");
            return;
        }

        printf("%s\n", "Received connection");
        spawn_child_process(server_sock_fd, incoming_sock_fd);
    // }
    close(server_sock_fd);
}

void spawn_child_process(int server_sock_fd, int incoming_sock_fd) {
    pid_t pid;
    time_t t;
    int status;

    if ((pid = fork()) < 0) {
        perror("fork() error");
        return;
    }
    
    if (pid == 0) { // this is done by the child process
        int valid_credentials = check_credentials(incoming_sock_fd); 
        if (valid_credentials) {

            // tell the client that the credentials are good and now it can send the command 
            char *check_credentials_ok = "ok";
            int write_ret = write(incoming_sock_fd, check_credentials_ok, strlen(check_credentials_ok));
            if (write_ret == -1) {
                perror("write() failed");
                return;
            }

            // now execute the command
            exec_command(incoming_sock_fd);
        } else {

        }

    } else do { // this is done by the parent process
        if ((pid = waitpid(pid, &status, WNOHANG)) == -1)
            perror("wait() error");
        else if (pid == 0) {
            time(&t);
            printf("child is still running at %s", ctime(&t));
            sleep(1);
        }
        else {
            if (WIFEXITED(status))
                printf("child exited with status of %d\n", WEXITSTATUS(status));
            else 
                puts("child did not exit successfully");
        }
    } while (pid == 0);
}

void exec_command(int incoming_sock_fd) {
    char command[BUFFER_SIZE];
    bzero(command,BUFFER_SIZE);
    int bytes_read = read(incoming_sock_fd,command,BUFFER_SIZE-1); // -1 for null terminator
    if (bytes_read < 0) {
        perror("read() failed");
        return;
    }
    command[bytes_read] = '\0'; // do this so we can print as string
    printf("Here is the command: %s\n",command);

    // now execute the command
    FILE *fp;
    int status;
    char buffer[BUFFER_SIZE];

    fp = popen(command, "r"); // popen does fork() when it runs command
    if (fp == NULL) {
        status = pclose(fp);
        close(incoming_sock_fd);
        fprintf(stderr, "%s\n", "Unknown command");
        return;
    }

    while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        int write_ret = write(incoming_sock_fd, buffer, BUFFER_SIZE-1);
        if (write_ret == -1) {
            status = pclose(fp);
            close(incoming_sock_fd);
            perror("write() failed");
            return;
        }
    }
    status = pclose(fp);
    close(incoming_sock_fd);
}

int check_credentials(int incoming_sock_fd) {
    char username[BUFFER_SIZE], encrypted_password_client[BUFFER_SIZE];
    bzero(username,BUFFER_SIZE);
    bzero(encrypted_password_client,BUFFER_SIZE);
    int bytes_read = read(incoming_sock_fd,username,BUFFER_SIZE-1); // -1 for null terminator
    if (bytes_read < 0) {
        perror("read() failed");
        return 0;
    }
    username[bytes_read] = '\0'; // do this so we can print as string

    // check username
    if (strcmp(username, DEFAULT_USERNAME) != 0) {
        fprintf(stderr, "%s\n", "Wrong username. Aborting...");
        return 0;
    }
    printf("Here is the username: %s\n",username);

    // generate random number 
    int random_num = generate_random_num();
    // convert to string
    char random_str[65]; 
    snprintf(random_str, 65, "%d", random_num);
    printf("%s\n", random_str);

    // send random number to the client
    int write_ret = write(incoming_sock_fd, random_str, sizeof(random_str));
    if (write_ret == -1) {
        perror("write() failed");
        return 0;
    }

    // read encrypted password sent by client
    bytes_read = read(incoming_sock_fd, encrypted_password_client, BUFFER_SIZE-1);
    if (bytes_read < 0) {
        perror("read() failed");
        return 0;
    }
    encrypted_password_client[bytes_read] = '\0'; // do this so we can print as string
    printf("Here is the encrypted password: %s\n",encrypted_password_client);

    // check encrypted password
    char *encrypted_password_server = crypt(DEFAULT_PASSWORD, random_str);
    printf("Here is the encrypted password on the server: %s\n", encrypted_password_server);
    if (strcmp(encrypted_password_server, encrypted_password_client) != 0) {
        fprintf(stderr, "%s\n", "Wrong password. Aborting...");
        return 0;
    }
    puts("Correct credentials!");
    return 1;
}

int generate_random_num() {
    srand(time(0)); // Use current time as seed for random generator
    return rand();
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