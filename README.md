# ft_ping

A reimplementation of the `ping` command in C, based on the inetutils-2.0 reference implementation.

## Description

`ft_ping` sends ICMP Echo Request packets to a network host and measures the round-trip time of each reply. It is built using raw sockets at the network layer (Layer 3), giving direct access to ICMP without going through TCP or UDP.

## How it works

1. **DNS Resolution** — the hostname is resolved to an IPv4 address via `getaddrinfo`
2. **Raw Socket** — a `SOCK_RAW` socket is created with `IPPROTO_ICMP`, giving direct access to ICMP
3. **Packet Building** — an ICMP Echo Request (type 8) is constructed with a header containing type, code, checksum, PID as ID, and an incrementing sequence number, plus a 56-byte payload
4. **Send/Receive Loop** — packets are sent every second via `sendto`, replies are received via `recvfrom`. The kernel prepends the IP header to received packets, from which TTL is extracted
5. **RTT Measurement** — `clock_gettime(CLOCK_MONOTONIC)` timestamps are captured before send and after receive to calculate round-trip time in milliseconds
6. **Signal Handling** — `SIGINT` (Ctrl+C) sets a global flag that exits the loop gracefully and prints statistics

## Usage

```bash
./ft_ping <hostname>
./ft_ping [-v] <hostname>
./ft_ping [-c count] <hostname>
./ft_ping -?
./ft_ping -V
```

## Options

| Flag | Description |
|------|-------------|
| `-v` | Verbose mode — prints error packets such as ICMP Time Exceeded |
| `-c N` | Stop after sending N packets |
| `-?` | Print help and available options |
| `-V` | Print version information |

## Output

```
PING google.com (172.217.20.46): 56 data bytes
64 bytes from 172.217.20.46: icmp_seq=0 ttl=254 time=13.285 ms
64 bytes from 172.217.20.46: icmp_seq=1 ttl=254 time=13.362 ms
^C--- google.com ping statistics ---
2 packets transmitted, 2 packets received, 0% packet loss
round-trip min/avg/max/stddev = 13.285/13.323/13.362/0.038 ms
```

## Implementation Details

- **ICMP checksum** — computed using RFC 1071 one's complement sum over the full packet
- **TTL extraction** — parsed from the IP header prepended to raw received packets
- **RTT statistics** — min/avg/max tracked per packet, stddev computed at exit using sum of squares
- **Timeout** — `SO_RCVTIMEO` set to 1 second on the socket to handle unreachable hosts
- **Sequence matching** — replies verified by checking type=0, id=PID, and sequence number

## Building

```bash
make        # build
make clean  # remove object files
make fclean # remove object files and binary
make re     # full rebuild
```

## Requirements

- Linux kernel > 3.14
- Root privileges or `CAP_NET_RAW` capability
- C compiler (cc/gcc)

## Author

Yahia Elboukili