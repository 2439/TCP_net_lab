#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "utils.h"

#define CLIENT_CHAR "./client"

void client(const char *ip);
void client_commands(int fd);
void client_up(int fd, cmd_t cmd);
void client_down(int fd, cmd_t cmd);

