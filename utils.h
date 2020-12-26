#include <stdint.h>
#include <netinet/in.h>
#include <string.h>

#define SERVER_PORT 1568 // 服务器十六进制端口号
#define BUFFER_MAX 2048
#define CMD_MAX_ARGS 5  // cmd最大段数
#define CMD_NAME_LEN 100    // cmd字符串整体长度
#define SUCCESS "success"
#define FAILED "failed"

typedef struct cmd {
    int argc;
    char *argv[CMD_MAX_ARGS];
    char buf[CMD_NAME_LEN];
} cmd_t;

// 设置addr，清空，设置sin_family,s_addr,port值
void set_addr(struct sockaddr_in *addr, int sin_family, in_addr_t s_addr, in_port_t port);

// 从终端获得命令
int getcmd(char* buf);

// 分析终端命令
void s_to_cmd(char *path, cmd_t* cmd);

// 清空缓存区
void flush_in();