# DrivePulse – Ethernet-Based Vehicle Monitoring System

A Linux socket-programming project implementing vehicle-to-dashboard communication in C using **TCP and UDP**. Built for the Automotive Communication Networks – Ethernet 2 assignment.

## Objectives
- Generate vehicle telemetry periodically in a simulated Vehicle ECU.
- Transmit vehicle data using UDP sockets.
- Implement reliable TCP client-server communication.
- Receive, parse and display vehicle information in a Dashboard ECU.
- Compare TCP and UDP behavior and inspect traffic with Wireshark.
- Validate normal operation, receiver-unavailable behavior and invalid configuration.

## Vehicle signals
- Vehicle speed (km/h)
- Engine RPM (rpm)
- Steering angle (degrees)

Each record also contains a sequence number, status and millisecond timestamp. Signals are deterministic simulated values, not live sensor measurements.

## Repository structure
```text
DrivePulse-Vehicle-Monitoring/
├── README.md
├── .gitignore
├── Makefile
├── generate_reports.py
├── src/                 # Vehicle ECU, Dashboard ECU and Makefile
├── docs/                # Design, validation and TCP/UDP comparison PDFs
├── results/             # Actual executed validation outputs
├── logs/                # Build, sender and receiver logs
├── scripts/             # Ubuntu experiment runner
├── screenshots/         # Genuine terminal/Wireshark screenshots
└── wireshark/           # Genuine TCP and UDP packet captures
```

## Build
On Ubuntu/Linux:
```bash
sudo apt update
sudo apt install -y build-essential wireshark tcpdump
make
```
The root Makefile creates `build/vehicle_ecu` and `build/dashboard_ecu`. You can also build directly from `src/` using `make -C src`.

## Run using UDP
Terminal 1 – Dashboard ECU:
```bash
./build/dashboard_ecu --protocol udp --bind 127.0.0.1 --port 5005
```
Terminal 2 – Vehicle ECU:
```bash
./build/vehicle_ecu --protocol udp --host 127.0.0.1 --port 5005 --count 20
```

## Run using TCP
Stop the UDP receiver, then start the TCP receiver in Terminal 1:
```bash
./build/dashboard_ecu --protocol tcp --bind 127.0.0.1 --port 5005
```
Terminal 2:
```bash
./build/vehicle_ecu --protocol tcp --host 127.0.0.1 --port 5005 --count 20
```
The sender transmits one record approximately every second. Use `--count 0` for continuous operation.

Command-line options:
- Vehicle ECU: `--protocol tcp|udp --host IPv4 [--port N] [--count N] [--interval-ms N]`
- Dashboard ECU: `--protocol tcp|udp [--bind IPv4] [--port N]`

## Automated experiment and packet capture
Run on Ubuntu with tcpdump installed and permission to capture on loopback:
```bash
chmod +x scripts/run_experiments.sh
./scripts/run_experiments.sh
``
The script builds the applications, runs TCP and UDP sessions, records terminal logs and attempts to save packet captures in `wireshark/`. Verify the resulting PCAP files in Wireshark before submission.

Useful display filters:
| Purpose | Filter |
|---|---|
| UDP traffic | `udp.port == 5005` |
| TCP traffic | `tcp.port == 5005` |
| TCP SYN | `tcp.flags.syn == 1` |
| TCP SYN-ACK | `tcp.flags.syn == 1 && tcp.flags.ack == 1` |
| TCP FIN | `tcp.flags.fin == 1` |

## Validation status
The included `results/validation_summary.txt` records executed tests: clean compilation, five UDP telemetry records received, five TCP telemetry records received, UDP behavior with no receiver, TCP connection refusal with no receiver, and invalid IPv4 input rejection.

**Packet-capture evidence status:** this project-generation runtime did not provide raw packet-capture permission and tcpdump, so genuine PCAP files and Wireshark screenshots could not be created here. They are not fabricated or represented as completed. Generate them on Ubuntu using the experiment script, then add verified files to `wireshark/` and genuine screenshots to `screenshots/`.

## Technical scope
This is an educational Linux application-layer socket demonstration over IPv4 loopback (`lo`). It is not a physical Ethernet-wire capture, Ethernet PHY implementation, 100BASE-T1, SOME/IP, CAN, AUTOSAR, or a production/safety-certified automotive ECU.

## Reports
The `docs/` directory contains the design document, validation report and TCP/UDP comparison as PDFs. The validation report distinguishes executed application tests from packet-level evidence that still requires a capture-capable Ubuntu host.
