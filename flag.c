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