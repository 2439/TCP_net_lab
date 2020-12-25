#include "server.h"

int main(int argc, char *argv[])
{
    // 输入判断
    if (argc != 1 || memcmp(argv[0], SERVER_CHAR, sizeof(SERVER_CHAR)) != 0)
    {
        printf("input error\n");
        printf("please input: server\n");
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
    if((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        printf("error create socket\n");
        return -1;
    }

    // bind()
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(SERVER_PORT);
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
    while(1) {
        if((clid = accept(fd, (struct sockaddr*)(&cli_addr), &addrlen)) == -1) {
            perror("Accept");
            exit(1);
        } else {
            printf("accept success %08x\n", ip_to_s(cli_addr.sin_addr));
            pthread_create(&thread_do, NULL, (void*)handle_request, &clid);
        }
    }
}

void handle_request(void *argv) {
    int clid = *((int*)argv);
    char buff[BUFFER_MAX];
    
}