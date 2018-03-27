#ifndef CLIENT_H
#define CLIENT_H

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

static const char *DEFAULT_PORT = "9898";
static const char *DEFAULT_USERNAME = "huyen";
static const char *DEFAULT_PASSWORD = "abc";

struct arguments {
   char *command;
   char *host;
   char *port;
} args;

void print_usage();
void parse_args(int argc, char **argv);
void establish_connection();
void initialize_default_values();
int signin(int sock_fd);

#endif