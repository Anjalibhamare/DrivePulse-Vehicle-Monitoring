# Experiment Screenshot Evidence

This folder is reserved for genuine screenshots captured while running DrivePulse on Ubuntu.

## Screenshots to add

1. **TCP communication:** Show the vehicle ECU terminal sending telemetry and the dashboard ECU receiving it.
2. **UDP communication:** Show the vehicle ECU terminal sending telemetry and the dashboard ECU receiving it.
3. **Wireshark TCP capture:** Show the captured loopback TCP traffic on port 5005.
4. **Wireshark UDP capture:** Show the captured loopback UDP traffic on port 5005.
5. **Build and validation:** Show the project compiling and the experiment runner completing.

## Capture instructions

- Run the application on Ubuntu.
- Start a genuine capture on the loopback interface (`lo`) using Wireshark.
- Use the display filter `tcp.port == 5005` for TCP or `udp.port == 5005` for UDP.
- Save clear screenshots in this folder with descriptive names, such as `tcp_terminal.png`, `udp_terminal.png`, `wireshark_tcp.png`, and `wireshark_udp.png`.

**Evidence status:** This file is an evidence checklist, not a screenshot. No screenshots are claimed to be present until actual experiment screenshots are added.
