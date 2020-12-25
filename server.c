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

    // accept
    // while(1) {
    // for(int i=0;i<10;i++){
        // printf("%d\n",i);
        if((clid = accept(fd, (struct sockaddr*)(&cli_addr), &addrlen)) == -1) {
            perror("Accept");
            exit(1);
        } else {
            printf("accept success %s\n", inet_ntoa(cli_addr.sin_addr));
            // read() and write()
            pthread_create(&thread_do, NULL, (void*)handle_request, &clid);
        }
    // }
    // sleep(10);
    system("netstat -an | grep 1568");	// 查看连接状态
    return;
}

void handle_request(void *argv) {
    int clid = *((int *)argv);
    char buf[BUFFER_MAX];
    printf("%lu\n", syscall(SYS_gettid));
    
    read(clid, buf, sizeof(buf));
    printf("%s\n",buf);
    return;
}