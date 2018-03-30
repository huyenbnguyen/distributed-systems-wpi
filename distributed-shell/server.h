/*
 * Name: Huyen Nguyen
 * Distributed Systems @ WPI (2018)
 *
 */

#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/wait.h>
#include <time.h>

#define QUEUE_LIMIT 5
#define BUFFER_SIZE 1024

const char *DEFAULT_PORT = "9898";
const char *DEFAULT_USERNAME = "huyen";
const char *DEFAULT_PASSWORD = "abc";

struct arguments {
   char *current_directory;
   const char *port;
} args;

void print_usage(char *current_directory);
void parse_args(int argc, char **argv, char *current_directory);
char *get_cwd();
void establish_connection();
void spawn_child_process(int server_sock_fd, int incoming_sock_fd);
int check_credentials(int incoming_sock_fd);
int generate_random_num();
void exec_command(int incoming_sock_fd);

#endif