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


uint16_t compute_checksum(void *data, int len)
{
    uint16_t    *ptr;
    uint32_t    sum;

    ptr = data;
    sum = 0;
    while (len > 1)
    {
        sum += *ptr++;
        len -= 2;
    }
    if (len == 1)
        sum += *(uint8_t *)ptr;
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return (~sum); //bit flip
}

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
    t_icmp_header   header;

    // test checksum
    header.type     = 8;
    header.code     = 0;
    header.id       = 1;
    header.sequence = 1;
    header.checksum = 0;
    header.checksum = compute_checksum(&header, sizeof(t_icmp_header));
    printf("checksum: 0x%x\n", header.checksum);

    uint16_t verify = compute_checksum(&header, sizeof(t_icmp_header));
    printf("verification raw: 0x%x\n", verify);
    printf("verification + checksum: 0x%x\n", (uint16_t)(header.checksum + verify));
    // int sock = init_sock();
    // if (sock < 0)
    //     return (1);
    // printf("socket opened successfully\n");
    // close(sock);
    return (0);
}