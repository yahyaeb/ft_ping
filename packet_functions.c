#include "lib.h"


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

int    receive_ping(int sock, int sequence)
{
    t_recv_buffer       buffer;
    struct sockaddr_in  sender;
    socklen_t           sender_len;
    ssize_t             bytes;

    sender_len = sizeof(sender);
    bytes = recvfrom(sock, &buffer, sizeof(buffer), 0,
                    (struct sockaddr *)&sender, &sender_len);
    if (bytes < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
            return (-1);
        perror("recvfrom");
        return (-1);
    }
    if (buffer.icmp.header.type == 0 &&
        buffer.icmp.header.id == getpid() &&
        buffer.icmp.header.sequence == sequence)
        return (buffer.ip.ttl);
    if (g_verbose)
    {
        if (buffer.icmp.header.type == 11)
            printf("From %s: icmp_seq=%d Time to live exceeded\n",
                inet_ntoa(sender.sin_addr),
                sequence);
        else
            printf("From %s: icmp_seq=%d type=%d code=%d\n",
                inet_ntoa(sender.sin_addr),
                sequence,
                buffer.icmp.header.type,
                buffer.icmp.header.code);
    }
    return (-2);
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
    // printf("packet sent successfully\n");
}
