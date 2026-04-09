#include "lib.h"

int g_verbose = 0;
int g_running = 1;

void handle_sigint(int sig)
{
    (void)sig;
    g_running = 0;
}

double update_rtt(t_stats *stats, struct timespec *start, struct timespec *end)
{
    double rtt;

    rtt = (end->tv_sec - start->tv_sec) * 1000.0 //converts to miliseconds
        + (end->tv_nsec - start->tv_nsec) / 1e6; // converts the nano seconds difference to milisenconds
    if (rtt < stats->rtt_min)
        stats->rtt_min = rtt;
    if (rtt > stats->rtt_max)
        stats->rtt_max = rtt;
    stats->rtt_sum += rtt; // avg
    stats->rtt_sum_sq += rtt * rtt; //stddev, low value is stable, high value is unstable
    return (rtt);
}

int main(int argc, char **argv)
{
    int                 sock;
    t_icmp_packet       packet;
    struct sockaddr_in  dest;
    struct timespec     start, end;
    t_stats             stats = {0, 0, 0, INT_MAX, 0, 0, 0};
    double              rtt;
    int                 ttl;
    int                 count;
    char                *hostname;

    count    = 0;
    hostname = parse_args(argc, argv, &count);
    dest     = resolve_host(hostname);
    sock     = init_sock();
    if (sock < 0)
        return (1);
    if (g_verbose)
        printf("PING %s (%s): 56 data bytes, id 0x%04x = %d\n",
            hostname, inet_ntoa(dest.sin_addr),
            (uint16_t)getpid(), (uint16_t)getpid());
    else
        printf("PING %s (%s): 56 data bytes\n",
            hostname, inet_ntoa(dest.sin_addr));

    signal(SIGINT, handle_sigint);
    memset(&packet, 0, sizeof(packet));

    while (g_running)
    {
        if (count > 0 && stats.packets_sent >= count)
            break;
        packet = build_packet(packet.header.sequence);
        clock_gettime(CLOCK_MONOTONIC, &start);
        send_ping(sock, &packet, &dest);
        stats.packets_sent++;
        ttl = receive_ping(sock, packet.header.sequence);
        if (ttl >= 0)
        {
            clock_gettime(CLOCK_MONOTONIC, &end);
            rtt = update_rtt(&stats, &start, &end);
            printf("64 bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n",
                inet_ntoa(dest.sin_addr),
                packet.header.sequence,
                ttl, rtt);
            stats.packets_received++;
        }
        else if (ttl == -2)
            stats.errors++;
        packet.header.sequence++;
        sleep(1);
    }
    print_stats(&stats, hostname);
    close(sock);
    return (0);
}
