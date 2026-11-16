# TCP and UDP Comparison

| Feature | TCP | UDP |
|---|---|---|
| Connection | Connection-oriented | Connectionless |
| Delivery | Reliable, ordered byte stream | Best-effort datagrams |
| Message boundaries | Not preserved; application framing required | Preserved per datagram |
| Handshake | Three-way connection handshake | No transport handshake |
| Delivery confirmation | Acknowledgements and retransmissions | No built-in delivery acknowledgement |
| Typical trade-off | Reliability and ordering | Lower protocol overhead and no connection setup |

## In this project
The same telemetry fields are sent using both transports. TCP records are separated using newline delimiters because TCP is a byte stream. UDP sends one record per datagram. A successful UDP `sendto()` does not prove that the receiver received the datagram.

## Wireshark observations
For TCP, inspect SYN, SYN-ACK, ACK, data segments, acknowledgements and connection termination. For UDP, inspect individual datagrams and their source/destination ports. Use `tcp.port == 5005` and `udp.port == 5005`.

## Automotive context
The choice of transport depends on application timing, reliability and fault-handling requirements. This educational example is not a safety design or a standards-compliant automotive Ethernet implementation.
