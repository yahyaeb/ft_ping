#include "lib.h"

void print_options(void)
{
    printf("Usage: ft_ping [OPTION...] HOST ...\n");
    printf("Send ICMP ECHO_REQUEST packets to network hosts.\n");
    printf("\n Options controlling ICMP request types:\n");
    printf("      --echo                 send ICMP_ECHO packets (default)\n");
    printf("\n Options valid for all request types:\n");
    printf("  -v, --verbose              verbose output\n");
    printf("  -?, --help                 give this help list\n");
    printf("  -V, --version              print program version\n");
}


void    print_version(void)
{
    printf("ft_ping (42 ft_ping) version: 1.0\n");
    printf("Copyright (C) 2025 %s\n", "yel-bouk");
}


void    print_stats(t_stats *stats, char *hostname)
{
    double avg;
    double stddev;

    printf("--- %s ping statistics ---\n", hostname);
    if (stats->packets_sent > 0)
    {
        if (stats->errors > 0)
            printf("%d packets transmitted, %d packets received, +%d errors, %d%% packet loss\n",
                stats->packets_sent,
                stats->packets_received,
                stats->errors,
                ((stats->packets_sent - stats->packets_received) * 100) / stats->packets_sent);
        else
            printf("%d packets transmitted, %d packets received, %d%% packet loss\n",
                stats->packets_sent,
                stats->packets_received,
                ((stats->packets_sent - stats->packets_received) * 100) / stats->packets_sent);
        if (stats->packets_received > 0)
        {
            avg    = stats->rtt_sum / stats->packets_received;
            stddev = sqrt((stats->rtt_sum_sq / stats->packets_received) - (avg * avg));
            printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n",
                stats->rtt_min, avg, stats->rtt_max, stddev);
        }
    }
}
