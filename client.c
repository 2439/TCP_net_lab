#include "client.h"

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
    client_commands(fd);
    close(fd);
}

// 客户机请求
void client_commands(int fd) {
    char buf[BUFFER_MAX];
    cmd_t cmd;

    memset(buf, 0, sizeof(buf));
    system("mkdir clientfile");

    while(getcmd(buf) >= 0) {
        s_to_cmd(buf, &cmd);
        if(cmd.argc == 0) {
            continue;
        }
        if(strcmp(cmd.argv[0], UP) == 0) {
            client_up(fd, cmd);
            continue;
        }
        if(strcmp(cmd.argv[0], DOWN) == 0) {
            client_down(fd, cmd);
            continue;
        }
        if(strcmp(cmd.argv[0], EXIT) == 0) {
            my_write(fd, cmd.argv[0], strlen(cmd.argv[0]));
            sleep(1);
            return;
        }
        system(buf);
        memset(buf, 0, sizeof(buf));
    }
}

// 客户端上传
void client_up(int fd, cmd_t cmd) {
    FILE *fp = fopen(cmd.argv[1], "r");

    // cmd长度判断，up 上传客户端文件 上传文件服务器位置
    if(cmd.argc != 3) {
        printf("error command\n");
        printf("It should be up /cilent/path.../upfilename /server/path...\n");
        return;
    }

    // 文件打开
    if (fp == NULL) {
        printf("Open %s error\n", cmd.argv[1]);
        return;
    } else {
        printf("open %s success\n", cmd.argv[1]);
    }

    // 命令,文件名上传
    my_write(fd, cmd.argv[0], strlen(cmd.argv[0]));
    cat1_name_to2(&cmd);
    my_write(fd, cmd.argv[2], strlen(cmd.argv[2]));   

    // 文件内容上传
    write_file(fd, fp);
    fclose(fp);
}

// 客户端下载
void client_down(int fd, cmd_t cmd) {
    FILE *fp;
    
    // cmd长度判断，down 服务器端下载文件 客户端文件位置
    if(cmd.argc != 3) {
        printf("error command\n");
        printf("It should be down /server/path.../downname /client/path...\n");
        return;
    }

    // 文件打开
    cat1_name_to2(&cmd);
    fp = fopen(cmd.argv[2], "w");
    if (fp == NULL) {
        printf("Open %s error\n", cmd.argv[2]);
        return;
    } else {
        printf("open %s success\n", cmd.argv[2]);
    }

    // 下载文件请求命令，文件位置
    my_write(fd, cmd.argv[0], strlen(cmd.argv[0]));
    my_write(fd, cmd.argv[1], strlen(cmd.argv[1]));  

    // 读取文件
    read_file(fd, fp);
    fclose(fp);
}
