#include <stdint.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>

#define SERVER_PORT 1568 // 服务器十六进制端口号
#define BUFFER_MAX 2048
#define CMD_MAX_ARGS 5  // cmd最大段数
#define SUCCESS "success"
#define FAILED "failed"
#define EXIT "exit"
#define OK "ok"

#define UP "up"
#define DOWN "down"

typedef struct cmd {
    int argc;
    char *argv[CMD_MAX_ARGS];
    char buf[BUFFER_MAX];
} cmd_t;

// 设置addr，清空，设置sin_family,s_addr,port值
void set_addr(struct sockaddr_in *addr, int sin_family, in_addr_t s_addr, in_port_t port);

// 从终端获得命令
int getcmd(char* buf);

// 分析终端命令
void s_to_cmd(char *path, cmd_t* cmd);

// 清空缓存区
void flush_in();

// 从fp中读取数据，写入fd中，传输
void write_file(int fd, FILE* fp);

// 从fd中读取数据，写入fp中
void read_file(int fd, FILE* fp);

// // 打印传输结果
// void read_result(int fd);

// // 写入传输结果
// void write_result(int fd, char* result);

// 把cmd.argv[1]的文件名写到cmd.argv[2]的文件名后面
void cat1_name_to2(cmd_t *cmd);

// 保证完全写
int my_write(int fd, char *buffer, int length);

// 保证完全读
int my_read(int fd, char *buffer, int length);