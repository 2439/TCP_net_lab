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
    closesocket(fd);
    return;
}

// 客户机请求
void client_commands(int fd) {
    char buf[BUFFER_MAX];
    cmt_t cmd;
    struct fcb_struct blk;

    write(fd, buf, sizeof(buf));
    system("mkdir clientfile");

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
    FILE *fp = fopen(cmd.argv[1], "rb");

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
    write(fd, cmd.argv[0], sizeof(cmd.argv[0]));
    cat1_name_to2(&cmd);
    write(fd, cmd.argv[2], sizeof(cmd.argv[2]));    

    // 文件内容上传
    write_file(fd, fp);
    fclose(fp);
    
    // 文件传输失败或成功
    printf("up file ");
    read_result(fd);
}

// 客户端下载
void client_down(int fd, char *buf, cmd_t cmd) {
    FILE *fp;
    
    // cmd长度判断，down 服务器端下载文件 客户端文件位置
    if(cmd.argc != 3) {
        printf("error command\n");
        printf("It should be down /server/path.../downname /client/path...\n");
        return;
    }

    // 文件打开
    cat1_name_to2(&cmd);
    fp =  = fopen(cmd.argv[2], "wb");
    if (fp == NULL) {
        printf("Open %s error\n", cmd.argv[2]);
        return;
    } else {
        printf("open %s success\n", cmd.argv[2]);
    }

    // 下载文件请求命令，文件位置
    write(fd, cmd.argv[0], sizeof(cmd.argv[0]));
    write(fd, cmd.argv[1], sizeof(cmd.argv[1]));  

    // 读取文件
    if(read_file(fd, fp) < 0) {
        printf("down file %s\n", FAILED);
        write(fd, FAILED, sizeof(FAILED));
    } else {
        printf("down file %s\n", SUCCESS);
        write(fd, SUCCESS, sizeof(SUCCESS));
    }
    fclose(fp);

}

// 把cmd.argv[1]的文件名写到cmd.argv[2]的文件名后面
void cat1_name_to2(cmd_t *cmd) {
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];

    _splitpath(cmd.argv[1], NULL, NULL, fname, ext);
    strcat(cmd.argv[2], fname);
    strcat(cmd.argv[2], ext);
}

// 打印传输结果
void read_result(int fd) {
    int buf[BUFFER_MAX];

    read(fd, buf, sizeof(buf));
    printf("%s\n", buf);
}