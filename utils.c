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
    memset(buf, 0, BUFFER_MAX);
    printf("==> ");

    while(!scanf("%[^\n]",buf)) {
        flush_in();
        max_enter--;
        if(max_enter == 0) {
            memcpy(buf, EXIT, sizeof(EXIT));
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
    char whitespace[] = " \t\r\n";

    memset(cmd->buf, 0, sizeof(cmd->buf));
    memcpy(cmd->buf, path, strlen(path));
    while(strchr(whitespace, path[i]) && path[i]) {     // 空字符跳过
        i++;
    }
    while(path[i] != 0) {
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

// 从fp中读取数据，写入fd中，传输
void write_file(int fd, FILE* fp) {
    int len;
    char buf[BUFFER_MAX];
    memset(buf, 0, sizeof(buf));

    while(len = fread(buf, 1, sizeof(buf), fp) > 0) {
        my_write(fd, buf, strlen(buf));
        memset(buf, 0, sizeof(buf));
    }
    // my_write(fd, buf, BUFFER_MAX);
}

// 从fd中读取数据，写入fp中
void read_file(int fd, FILE* fp) {
    char buf[BUFFER_MAX];
    memset(buf, 0, sizeof(buf));

    my_read(fd, buf, sizeof(buf));
    fwrite(buf, 1, strlen(buf), fp);
    printf("read_file\n");
}

// 把cmd.argv[1]的文件名写到cmd.argv[2]的文件名后面
void cat1_name_to2(cmd_t *cmd) {
    char* fname;

    fname = strrchr(cmd->argv[1], '/') + 1;
    strcat(cmd->argv[2], "/");
    strcat(cmd->argv[2], fname);
    cmd->argv[2][strlen(cmd->argv[2])] = 0;
}

// 保证完全写
int my_write(int fd, char *buffer, int length) {
    int left = length;
    int w = 0;
    char *p = buffer;

    printf("write %s\n", buffer);
    while(left > 0) {
        w = write(fd, p, left);
        sleep(1);
        if(w < 0) {
            return -1;
        }
        left -= w;
        p += w;
        if(p == 0) {
            break;
        }
    }
    return length - left;
}

// 保证完全读
int my_read(int fd, char *buffer, int length) {
    int left = length;
    int r = 0;
    char *p = buffer;

    memset(buffer, 0, BUFFER_MAX);
    while(left > 0) {
        r = read(fd, p, left);
        sleep(1);
        if(r < 0) {
            return -1;
        }
        left -= r;
        p += r;
        if(*p == 0) {
            break;
        }
    }
    printf("read: %s\n", buffer);
    return length - left;
}

// 得到ok消息
int get_ok(int fd) {
    char buf[BUFFER_MAX];

    my_read(fd, buf, sizeof(buf));
    if(memcmp(buf, OK, sizeof(OK)) == 0) {
        return 0;
    } else {
        return -1;
    }
}