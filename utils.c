#include "utils.h"

// 设置地址的端口
void set_addr(struct sockaddr_in *addr, int sin_family, in_addr_t s_addr, in_port_t port) {
    memset(addr, 0, sizeof(struct sockaddr_in));
    addr->sin_family = sin_family;
    addr->sin_addr.s_addr = s_addr;
    addr->sin_port = port;
}

// 从终端获得命令
int getcmd(char *buf) {
    int max_enter = 10;
    memset(buf, 0, CMD_NAME_LEN);
    printf("==> ")；

    while(!scanf("%[^\n]",buf)) {
        flush_in();
        max_enter--;
        if(max_enter == 0) {
            return -1;
        }
        printf("==> ");
    }
    flush_in();
    return 0;
}

// 分析终端命令
void s_to_cmd(char *path, cmd_t* cmd) {
    int count = 0;
    int i = 0;
    char whitespace[] = "\t\r\nv";

    memcpy(cmd->buf, path);
    while(strchr(whitespace, path[i]) && path[i]) {     // 空字符跳过
        i++;
    }

    while(path[i]) {
        cmd->argv[count++] = &cmd->buf[i];
        while(!strchr(whitespace, path[i]) && path[i]) {
            i++;
        }
        if(!path[i]) {
            break;
        }
        cmd->buf[i] = 0;
        i++;
        while(strchr(whitespace, path[i]) && path[i]) {
            i++;
        }
    }
    cmd->argv[count] = 0;
    cmd->argc = count;
}

// 清空缓存区
void flush_in() {
    int ch;
    while((ch = getchar()) != EOF && ch != '\n');
}