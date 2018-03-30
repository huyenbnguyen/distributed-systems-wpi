#ifndef CLIENT_H
#define CLIENT_H

#define _XOPEN_SOURCE 
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

const char *DEFAULT_PORT = "9898";
const char *DEFAULT_USERNAME = "huyen";
const char *DEFAULT_PASSWORD = "abc";

struct arguments {
   char *command;
   char *host;
   const char *port;
} args;

void print_usage();
void parse_args(int argc, char **argv);
void establish_connection();
int signin(int sock_fd);
void run_command_on_server(int sock_fd); 

#endif