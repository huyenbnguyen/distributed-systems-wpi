/*
 * Name: Huyen Nguyen
 * Distributed Systems @ WPI (2018)
 *
 */

#include "shared.h"
#include "server.h"

int main(int argc, char **argv) {
    get_cwd();

    parse_args(argc, argv);
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

    int server_sock_fd; 
    int new_port = 1024;
    while ((server_sock_fd = create_socket()) < 0 && new_port <= MAX_SOCKET_NUM) {
        printf("Port %s is being used.\n", args.port);
        fprintf(stderr, "%s\n", "Trying a different port number...");
        int new_port = atoi(args.port) + 1;
        // convert to string
        char random_str[17]; // port number can only be within 0 to 2^16-1
        snprintf(random_str, 17, "%d", new_port);
        args.port = random_str;
    }
    printf("SOCKET CREATED SUCCESSFULLY! PORT NUMBER IS %s\n", args.port);
    printf("%s\n", "./server activating");
    printf("\tPORT:%s\n", args.port);
    printf("\tdirectory:%s\n", args.current_directory);

    listen_request(server_sock_fd);
}

void listen_request(int server_sock_fd) {
    int incoming_sock_fd;
    socklen_t clilen;
    struct sockaddr_in cli_addr;
    
    printf("Socket ready to go! Accepting connections....\n\n");
    clilen = sizeof(cli_addr);

    while (1) {
        puts("\n************Listening for requests...************");
        /* wait here (block) for connection */ 
        incoming_sock_fd = accept(server_sock_fd, (struct sockaddr *) &cli_addr, &clilen);
        if (incoming_sock_fd < 0) {
            perror("accept() failed");
            return;
        }
        printf("%s\n", "Received connection");
        spawn_child_process(server_sock_fd, incoming_sock_fd);
    }
}

int create_socket() {
    int server_sock_fd, incoming_sock_fd, getaddrinfo_ret;
    struct addrinfo hints, *servinfo;

    // getaddrinfo() replaces gethostbyname()
    // see here for the reason: https://beej.us/guide/bgnet/html/multi/getaddrinfoman.html  
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // allow hostname to be NULL
    if ((getaddrinfo_ret = getaddrinfo(NULL, args.port, &hints, &servinfo)) != 0) {
        freeaddrinfo(servinfo);
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(getaddrinfo_ret));
        return -1;
    }  

    /* create socket from which to read */
    if ((server_sock_fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) < 0) {
        freeaddrinfo(servinfo);
        perror("socket() failed");
        return -1;
    }

    /* bind our local address so client can connect to us */
    if (bind(server_sock_fd, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
        freeaddrinfo(servinfo);
        perror("bind() failed");
        return -1;
    }

    freeaddrinfo(servinfo);

    /* mark socket as passive, with backlog num */
    if (listen(server_sock_fd, QUEUE_LIMIT) == -1) {
        perror("listen() failed");
        return -1;
    }
    return server_sock_fd;
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
        int invalid_credentials = check_credentials(incoming_sock_fd); 
        if (!invalid_credentials) {

            // tell the client that the credentials are good and now it can send the command 
            char *check_credentials_ok = "ok";
            int write_ret = write(incoming_sock_fd, check_credentials_ok, strlen(check_credentials_ok));
            if (write_ret == -1) {
                perror("write() failed");
                return;
            }

            // now execute the command
            exec_command(server_sock_fd, incoming_sock_fd);
        } else {
            puts("Invalid credentials. Aborting...");
            exit(1);
        }

    } else do { // this is done by the parent process
        if ((pid = waitpid(pid, &status, WNOHANG)) == -1)
            perror("wait() error");
        else if (pid == 0) {
            time(&t);
            // uncomment these lines for debugging purposes
            // printf("child with pid %d is still running at %s", pid, ctime(&t));
            // sleep(1);
        }
        else {
            if (WIFEXITED(status))
                printf("child with pid %d exited with status of %d\n", pid, WEXITSTATUS(status));
            else 
                puts("child did not exit successfully");
        }
    } while (pid == 0);
}

void exec_command(int server_sock_fd, int incoming_sock_fd) {
    char command[BUFFER_SIZE];
    bzero(command,BUFFER_SIZE);
    int bytes_read = read(incoming_sock_fd,command,BUFFER_SIZE-1); // -1 for null terminator
    if (bytes_read < 0) {
        perror("read() failed");
        return;
    }
    command[bytes_read] = '\0'; // do this so we can print as string
    printf("Here is the command: %s\n",command);

    if (strcmp(command, "exit") == 0) {
        puts("Server exiting...");
        free(current_directory);
        close(incoming_sock_fd);
        close(server_sock_fd);
        kill(0, SIGKILL);
    }

    // now execute the command
    FILE *fp;
    int status;
    char buffer[BUFFER_SIZE];

    fp = popen(command, "r"); // popen does fork() when it runs command
    if (fp == NULL) {
        close(incoming_sock_fd);
        close(server_sock_fd);
        fprintf(stderr, "%s\n", "Unknown command");
        exit(1);
    }

    // now send the output to the client
    while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        int write_ret = write(incoming_sock_fd, buffer, BUFFER_SIZE-1);
        if (write_ret == -1) {
            status = pclose(fp);
            close(incoming_sock_fd);
            close(server_sock_fd);
            perror("write() failed");
            exit(1);
        }
    }
    status = pclose(fp);
    if (status == -1) {
        close(incoming_sock_fd);
        close(server_sock_fd);
        perror("pclose() failed");
        exit(1);
    }
    close(incoming_sock_fd);
    exit(0);
}

int check_credentials(int incoming_sock_fd) {
    char username[BUFFER_SIZE], encrypted_password_client[BUFFER_SIZE];
    bzero(username,BUFFER_SIZE);
    bzero(encrypted_password_client,BUFFER_SIZE);
    int bytes_read = read(incoming_sock_fd,username,BUFFER_SIZE-1); // -1 for null terminator
    if (bytes_read < 0) {
        perror("read() failed");
        return 1;
    }
    username[bytes_read] = '\0'; // do this so we can print as string

    // check username
    if (strcmp(username, default_username) != 0) {
        fprintf(stderr, "%s\n", "Wrong username. Aborting...");
        return 1;
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
        return 1;
    }

    // read encrypted password sent by client
    bytes_read = read(incoming_sock_fd, encrypted_password_client, BUFFER_SIZE-1);
    if (bytes_read < 0) {
        perror("read() failed");
        return 1;
    }
    encrypted_password_client[bytes_read] = '\0'; // do this so we can print as string
    printf("Here is the encrypted password: %s\n",encrypted_password_client);

    // check encrypted password
    char *encrypted_password_server = crypt(default_password, random_str);
    printf("Here is the encrypted password on the server: %s\n", encrypted_password_server);
    if (strcmp(encrypted_password_server, encrypted_password_client) != 0) {
        fprintf(stderr, "%s\n", "Wrong password.");
        return 1;
    }
    puts("Correct credentials!");
    return 0;
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

void parse_args(int argc, char **argv) {
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

void get_cwd() {
    while (1) {
        current_directory = (char *) malloc (size);
        if (getcwd (current_directory, size) == current_directory)
            return;
        free (current_directory);
        size *= 2;
    }
}
