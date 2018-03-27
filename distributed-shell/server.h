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

#define QUEUE_LIMIT 5

struct arguments {
   char *current_directory;
   char *port;
} args;

void print_usage(char *current_directory);
void parse_args(int argc, char **argv, char *current_directory);
char *get_cwd();
void establish_connection();

#endif