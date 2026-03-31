#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>

typedef struct s_icmp_header
{
    uint8_t     type;
    uint8_t     code;
    uint16_t    checksum;
    uint16_t    id;
    uint16_t    sequence;
}               t_icmp_header;

typedef struct s_icmp_packet
{
    t_icmp_header   header;
    uint8_t         payload[56];
}               t_icmp_packet;

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
    return (~sum);
}

struct sockaddr_in  resolve_host(char *hostname)
{
    struct addrinfo     hints;
    struct addrinfo     *res;
    struct sockaddr_in  dest;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    if (getaddrinfo(hostname, NULL, &hints, &res) != 0)
    {
        printf("ft_ping: unknown host %s\n", hostname);
        // better error handling later
    }
    dest = *(struct sockaddr_in *)res->ai_addr;
    freeaddrinfo(res);
    return dest;
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

t_icmp_packet   build_packet(int sequence)
{
    t_icmp_packet   packet;

    packet.header.type     = 8;
    packet.header.code     = 0;
    packet.header.id       = getpid();
    packet.header.sequence = sequence;
    packet.header.checksum = 0;
    memset(packet.payload, 0x42, 56);
    packet.header.checksum = compute_checksum(&packet, sizeof(t_icmp_packet));
    return packet;
}

void    send_ping(int sock, t_icmp_packet *packet, struct sockaddr_in *dest)
{
    ssize_t bytes;

    bytes = sendto(sock, packet, sizeof(t_icmp_packet), 0,
                   (struct sockaddr *)dest, sizeof(*dest));
    if (bytes < 0)
    {
        perror("sendto");
        return ;
    }
    printf("packet sent successfully\n");
}

int main(int argc, char **argv)
{
    int                 sock;
    t_icmp_packet       packet;
    struct sockaddr_in  dest;

    if (argc != 2)
    {
        printf("Usage: ./ft_ping <hostname>\n");
        return (1);
    }
    dest   = resolve_host(argv[1]);
    sock   = init_sock();
    if (sock < 0)
        return (1);
    packet = build_packet(1);
    printf("PING %s (%s)\n", argv[1], inet_ntoa(dest.sin_addr));
    send_ping(sock, &packet, &dest);
    close(sock);
    return (0);
}