#include "client.h"
#include "config.h"

int main(int argc, char *argv[])
{
    // 输入判断
    if (argc != 2 || memcmp(argv[0], CLIENT_CHAR, sizeof(CLIENT_CHAR)) != 0)
    {
        printf("input error\n");
        printf("please input: client server_ip\n");
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
    if ((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        printf("error create socket\n");
        return -1;
    }

    // connect()，链接服务器地址和端口
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(SERVER_PORT);
    if (connect(fd, (struct sockaddr*)(&addr), sizeof(struct sockaddr)) < 0)
    {
        perror("Connect Error:");
        exit(1);
    }
    else
    {
        printf("connect success!\n");
    }
}