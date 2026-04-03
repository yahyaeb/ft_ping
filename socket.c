#include "lib.h"

int init_sock(void)
{
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock < 0)
    {
        perror("socket");
        return (-1);
    }
    return sock;
}