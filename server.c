#include "server.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    // 输入判断
    if (argc != 1 || memcmp(argv[0], SERVER_CHAR, sizeof(SERVER_CHAR)) != 0)
    {
        printf("input error\n");
        printf("please input: ./server\n");
        return -1;
    }

    server();
    return 0;
}

// 服务器
void server() {
    int fd;
    struct sockaddr_in addr;
    struct sockaddr_in cli_addr;
    int clid;
    socklen_t addrlen = sizeof(struct sockaddr_in);
    pthread_t thread_do;

    // socket()
    if((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("error create socket\n");
        return;
    }

    // bind()
    set_addr(&addr, AF_INET, htonl(INADDR_ANY), htons(SERVER_PORT));

    if(bind(fd, (struct sockaddr*)(&addr), sizeof(struct sockaddr)) < 0) {
        perror("Bind");
        exit(1);
    } else {
        printf("bind success\n");
    }

    // listen
    if(listen(fd, BACKLOG_MAX) == -1) {
        perror("listen");
        exit(1);
    } else {
        printf("listen success\n");
    }

    system("mkdir serverfile");

    // accept
    while(1) {
        if((clid = accept(fd, (struct sockaddr*)(&cli_addr), &addrlen)) > 0) {
            printf("accept success %s\n", inet_ntoa(cli_addr.sin_addr));
            // read() and write()
            pthread_create(&thread_do, NULL, (void*)handle_request, &clid);
        }
        // system("netstat -an | grep 1568");	// 查看连接状态
    }
    close(fd);
    return;
}

// 子线程
void handle_request(void *argv) {
    int clid = *((int *)argv);
    char buf[BUFFER_MAX];
    
    while(1) {
        // printf("pid = %d\n",getpid());
        memset(buf, 0, sizeof(buf));
        my_read(clid, buf, sizeof(buf));
        printf("%s\n",buf);
        if(strcmp(buf, UP) == 0) {
            server_up(clid);
            continue;
        }
        if(strcmp(buf, DOWN) == 0) {
            server_down(clid);
            continue;
        }
        if(strcmp(buf, EXIT) == 0) {
            break;
        } else {
            continue;
        }
    }
    close(clid);
    printf("close\n");
    pthread_cancel(pthread_self());
}

// 上传服务器端
void server_up(int fd) {
    printf("up start\n");
    char buf[BUFFER_MAX];
    FILE *fp;

    // 文件打开写入
    memset(buf, 0, sizeof(buf));
    my_read(fd, buf, sizeof(buf));
    printf("%s\n",buf);
    fp = fopen(buf, "wb");
    if(fp == NULL) {
        printf("open %s error\n", buf);
    } else {
        printf("open %s success\n", buf);
    }

    read_file(fd, fp);
    fclose(fp);
    printf("up end\n");
}

// 从服务端下载
void server_down(int fd) {
    FILE *fp;
    char buf[BUFFER_MAX];

    memset(buf, 0, sizeof(buf));
    my_read(fd, buf, sizeof(buf));
    fp = fopen(buf, "rb");
    if(fp == NULL) {
        printf("open %s error\n", buf);
    } else {
        printf("open %s success\n", buf);
    }

    write_file(fd, fp);
    fclose(fp);
}