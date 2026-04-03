#include "lib.h"

struct sockaddr_in  resolve_host(char *hostname)
{
    struct addrinfo     hints; //hints for setting up prefrences...
    struct addrinfo     *res;
    struct sockaddr_in  dest;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    if (getaddrinfo(hostname, NULL, &hints, &res) != 0) // NULL could be 80 or http... but ICMP doesn´t use ports. Also The getaddrinfo make a DNS request that resolves the address
    {
        printf("ft_ping: unknown host %s\n", hostname);
        exit(1);
        // we'll handle this error better later
    }
    dest = *(struct sockaddr_in *)res->ai_addr;
    freeaddrinfo(res);
    return dest;
}
