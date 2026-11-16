# Wireshark Packet Capture

The application uses IPv4 loopback (`127.0.0.1`) and port `5005`. To create genuine packet evidence on Ubuntu:

1. Install `wireshark` and `tcpdump`.
2. Start a capture on interface `lo`.
3. Run the UDP receiver and sender; save the capture as `udp_capture.pcap`.
4. Repeat for TCP; save as `tcp_capture.pcap`.
5. Open each capture in Wireshark and apply `udp.port == 5005` or `tcp.port == 5005`.
6. Save screenshots showing the packet list and selected packet details in `screenshots/`.

**Current status:** this repository does not claim to contain genuine PCAP captures. The current execution environment did not provide packet-capture permission/tooling. Do not substitute fabricated PCAP files.
