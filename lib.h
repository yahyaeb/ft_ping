#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <math.h>
#include <errno.h>


extern int g_verbose;
extern int g_running;

typedef struct s_stats
{
    int     packets_sent;
    int     packets_received;
    int     errors;
    double  rtt_min;
    double  rtt_max;
    double  rtt_sum;
    double  rtt_sum_sq;
}           t_stats;

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


typedef struct s_ip_header
{
    uint8_t     ihl_version;  // version (4 bits) + header length (4 bits)
    uint8_t     tos;
    uint16_t    tot_len;
    uint16_t    id;
    uint16_t    frag_off;
    uint8_t     ttl;
    uint8_t     protocol;
    uint16_t    checksum;
    uint32_t    saddr;
    uint32_t    daddr;
}               t_ip_header;

typedef struct s_recv_buffer
{
    t_ip_header     ip;
    t_icmp_packet   icmp;
}               t_recv_buffer;

int init_sock(void);
struct sockaddr_in  resolve_host(char *hostname);
uint16_t compute_checksum(void *data, int len);
int    receive_ping(int sock, int sequence);
t_icmp_packet   build_packet(int sequence);
void    send_ping(int sock, t_icmp_packet *packet, struct sockaddr_in *dest);
void print_options(void);
void    print_version(void);
