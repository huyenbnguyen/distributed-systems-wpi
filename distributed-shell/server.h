/*
 * Name: Huyen Nguyen
 * Distributed Systems @ WPI (2018)
 *
 */

#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>
#include <strings.h>
#include <unistd.h>

#define QUEUE_LIMIT 5
#define MAX_SOCKET_NUM 65535

size_t size = 100;
char *current_directory;

struct arguments {
   char *current_directory;
   char *port;
} args;

void print_usage(char *current_directory);
void parse_args(int argc, char **argv);
void get_cwd();
void listen_request(int server_sock_fd);
void spawn_child_process(int server_sock_fd, int incoming_sock_fd);
int check_credentials(int incoming_sock_fd);
int generate_random_num();
void exec_command(int server_sock_fd, int incoming_sock_fd);
int create_socket();

#endif
