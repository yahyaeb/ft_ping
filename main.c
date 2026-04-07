#include "lib.h"

int g_running = 1;

void handle_sigint(int sig)
{
    (void)sig;
    g_running = 0;
}

void update_rtt(t_stats *stats, struct timespec *start, struct timespec *end)
{
    double rtt;

    rtt = (end->tv_sec - start->tv_sec) * 1000.0
        + (end->tv_nsec - start->tv_nsec) / 1e6;
    if (rtt < stats->rtt_min)
        stats->rtt_min = rtt;
    if (rtt > stats->rtt_max)
        stats->rtt_max = rtt;
    stats->rtt_sum += rtt;
    stats->rtt_sum_sq += rtt * rtt;
}

int main(int argc, char **argv)
{
    int                 sock;
    t_icmp_packet       packet;
    struct sockaddr_in  dest;
    struct timespec     start, end;
    t_stats             stats = {0, 0, 999999, 0, 0, 0};
    double              avg;
    double              stddev;
    double              rtt;
    int                 ttl;

    if (argc != 2)
    {
        printf("Usage: ./ft_ping <hostname>\n");
        return (1);
    }
    dest   = resolve_host(argv[1]);
    sock   = init_sock();
    if (sock < 0)
        return (1);
    packet = build_packet(0);
    printf("PING %s (%s): 56 data bytes\n", argv[1], inet_ntoa(dest.sin_addr));
    signal(SIGINT, handle_sigint);
    while (g_running)
    {
        packet = build_packet(packet.header.sequence);
        clock_gettime(CLOCK_MONOTONIC, &start);
        send_ping(sock, &packet, &dest);
        stats.packets_sent++;
        ttl = receive_ping(sock, packet.header.sequence);
        if (ttl >= 0)
        {
            clock_gettime(CLOCK_MONOTONIC, &end);
            update_rtt(&stats, &start, &end);
            rtt = (end.tv_sec - start.tv_sec) * 1000.0
                + (end.tv_nsec - start.tv_nsec) / 1e6;
            printf("64 bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n",
                inet_ntoa(dest.sin_addr),
                packet.header.sequence,
                ttl,
                rtt);
            stats.packets_received++;
        }
        packet.header.sequence++;
        sleep(1);
    }
    printf("--- %s ping statistics ---\n", argv[1]);
    if (stats.packets_sent > 0)
    {
        printf("%d packets transmitted, %d packets received, %d%% packet loss\n",
            stats.packets_sent,
            stats.packets_received,
            ((stats.packets_sent - stats.packets_received) * 100) / stats.packets_sent);
        if (stats.packets_received > 0)
        {
            avg    = stats.rtt_sum / stats.packets_received;
            stddev = sqrt((stats.rtt_sum_sq / stats.packets_received) - (avg * avg));
            printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n",
                stats.rtt_min, avg, stats.rtt_max, stddev);
        }
    }
    close(sock);
    return (0);
}
