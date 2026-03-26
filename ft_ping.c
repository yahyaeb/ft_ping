#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>

#include <stdint.h>

typedef struct s_icmp_header
{
    uint8_t     type;        // 1 byte — 8 for request, 0 for reply
    uint8_t     code;        // 1 byte — always 0 for ping
    uint16_t    checksum;    // 2 bytes — error detection
    uint16_t    id;          // 2 bytes — your process ID
    uint16_t    sequence;    // 2 bytes — increments each packet
}               t_icmp_header;

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

int main(void)
{
    int sock = init_sock();
    if (sock < 0)
        return (1);
    printf("socket opened successfully\n");
    close(sock);
    return (0);
}