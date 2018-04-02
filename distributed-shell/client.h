#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>

struct arguments {
   char *command;
   char *host;
   const char *port;
} args;

void print_usage();
void parse_args(int argc, char **argv);
void establish_connection();
int signin_fail(int sock_fd);
void run_command_on_server(int sock_fd); 

#endif
