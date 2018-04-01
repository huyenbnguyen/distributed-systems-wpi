#ifndef SHARED_H
#define SHARED_H

#define BUFFER_SIZE 1024

#define _XOPEN_SOURCE
#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

char *default_port = "1024";
char *default_username = "huyen";
char *default_password = "abc";

#endif 
