#include "lib.h"

int init_sock(void)
{
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    struct timeval      timeout;
    if (sock < 0)
    {
        perror("socket");
        return (-1);
    }
    timeout.tv_sec  = 1;
    timeout.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    return sock;
}