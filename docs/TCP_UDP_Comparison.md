# TCP vs UDP – DrivePulse Comparison

| Feature | TCP | UDP |
|---|---|---|
| Connection | Connection-oriented | Connectionless |
| Delivery | Reliable retransmission/acknowledgements | Best effort; no delivery guarantee |
| Ordering | Ordered byte stream | Datagrams may arrive out of order |
| Overhead | Connection setup and stream management | Smaller transport header and no handshake |
| Record boundaries | Application must frame records (newline used here) | Datagram boundaries preserved |
| DrivePulse use | Dashboard session with orderly close | Periodic telemetry datagrams |

## Wireshark observations
For TCP, inspect SYN, SYN-ACK, ACK, data transfer and FIN packets. For UDP, inspect each datagram and its source/destination ports. Capture genuine traffic on loopback interface `lo` using display filters `tcp.port == 5005` and `udp.port == 5005`.
