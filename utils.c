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
    printf("==> ");

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
        my_write(fd, buf, len);
        memset(buf, 0, sizeof(buf));
    }
}

// 从fd中读取数据，写入fp中
void read_file(int fd, FILE* fp) {
    int len;
    char buf[BUFFER_MAX];
    memset(buf, 0, sizeof(buf));

    while(len = my_read(fd, buf, sizeof(buf)) > 0) {
        fwrite(buf, 1, len, fp);
        memset(buf, 0, sizeof(buf));
    }
}

// 把cmd.argv[1]的文件名写到cmd.argv[2]的文件名后面
void cat1_name_to2(cmd_t *cmd) {
    char* fname;

    fname = strrchr(cmd->argv[1], '/') + 1;
    strcat(cmd->argv[2], "/");
    strcat(cmd->argv[2], fname);
}

// 保证完全写
int my_write(int fd, void *buffer, int length) {
    int left = length;
    int w = 0;
    char *p = buffer;

printf("length:%d\n",length);
    while(1) {
        w = write(fd, p, left);
        if(w < 0) {
            return -1;
        }
        left -= w;
        p += w;
        if(*(p-1) == 0) {
            break;
        }
    }
    return length - left;
}

// 保证完全读
int my_read(int fd, void *buffer, int length) {
    int left = length;
    int r = 0;
    char *p = buffer;

    while(left > 0) {printf("1\n");
        r = read(fd, p, left);
        printf("r:%d\n",r);
        if(r < 0) {printf("2\n");
            return -1;
        }
        left -= r;printf("3\n");
        p += r;
        printf("p:%s\n",p);
        if(*(p-1) == 0) {printf("4\n");
            break;
        }
    }printf("5\n");
    return length - left;
}