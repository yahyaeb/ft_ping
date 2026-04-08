#include "lib.h"

int init_sock(void)
{
    int             sock;
    struct timeval  timeout;
    // int             ttl_val;

    sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock < 0)
    {
        perror("socket");
        return (-1);
    }
    timeout.tv_sec  = 1;
    timeout.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    // ttl_val = 1;
    // setsockopt(sock, IPPROTO_IP, IP_TTL, &ttl_val, sizeof(ttl_val));
    return (sock);
}