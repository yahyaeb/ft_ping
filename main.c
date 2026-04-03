#include "lib.h"

int g_running = 1;

void handle_sigint(int sig)
{
    (void)sig;
    g_running = 0;
}

int main(int argc, char **argv)
{
    int                 sock;
    t_icmp_packet       packet;
    struct sockaddr_in  dest;
    // int packets_sent = 0;
    int packets_received = 0;
    int packets_sent = 0;

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
    signal(SIGINT, handle_sigint);
    while (g_running)
    {
        packet = build_packet(packet.header.sequence);
        send_ping(sock, &packet, &dest);
        packets_sent++;
        if (receive_ping(sock, packet.header.sequence))
            packets_received++;
        packet.header.sequence++;
        sleep(1);
    }      
    printf("\n--- %s ping statistics ---\n", argv[1]);
    if (packets_sent > 0)
        printf("%d packets transmitted, %d received, %d%% packet loss\n",
            packets_sent,
            packets_received,
            ((packets_sent - packets_received) * 100) / packets_sent);
    close(sock);
    return (0);
}