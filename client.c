#include "client.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    // 输入判断
    if (argc != 2 || memcmp(argv[0], CLIENT_CHAR, sizeof(CLIENT_CHAR)) != 0)
    {
        printf("input error\n");
        printf("please input: ./client server_ip\n");
        return -1;
    }

    client(argv[1]);
    return 0;
}

// 客户端
// ip：服务器ip地址
void client(const char *ip)
{
    int fd;                  // socket
    struct sockaddr_in addr; // 服务器信息结构体

    // socket()
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("error create socket\n");
        return;
    }

    // connect()，连接服务器地址和端口
    set_addr(&addr, AF_INET, inet_addr(ip), htons(SERVER_PORT));
    if (connect(fd, (struct sockaddr*)(&addr), sizeof(struct sockaddr)) < 0)
    {
        perror("Connect Error:");
        exit(1);
    }
    else
    {
        printf("connect success!\n");
    }

    // 识别命令与执行
    // system("netstat -an | grep 1568");	// 查看连接状态
    // sleep(10);
    client_commands(fd);
    return;
}

// 客户机请求
void client_commands(int fd) {
    char buf[BUFFER_MAX];
    cmt_t cmd;

    write(fd, buf, sizeof(buf));

    while(getcmd(buf) >= 0) {
        s_to_cmd(buf, cmd);
        if(cmd.argc == 0) {
            continue;
        }
        if(strcmp(cmd.argv[0], "up") == 0) {
            client_up(fd, buf, cmd);
            continue;
        }
        if(strcmp(cmd.argv[0], "down") == 0) {
            client_down(fd, buf, cmd);
            continue;
        }
        if(strcmp(cmd.argv[0], "shutdown") == 0) {
            break;
        }
        system(buf);
    }
}

// 客户端上传
void client_up(int fd, char *buf, cmd_t cmd) {
    if(cmd.argc != 3) {
        printf("error command\n");
        printf("It should be up ")
        return;
    }

}

// 客户端下载
void client_down(char *buf, cmd_t cmd) {

}