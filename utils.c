#include "utils.h"

// 设置地址的端口
void set_addr(struct sockaddr_in *addr, int sin_family, in_addr_t s_addr, in_port_t port) {
    memset(addr, 0, sizeof(struct sockaddr_in));
    addr->sin_family = sin_family;
    addr->sin_addr.s_addr = s_addr;
    addr->sin_port = port;
}

char *ip_to_s(uint32_t ip) {

}