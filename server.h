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
#include <pthread.h>
#include <sys/syscall.h>

#define BACKLOG_MAX 5
#define SERVER_CHAR "./server"

// 服务器
void server();
// 线程
void handle_request(void *argv);