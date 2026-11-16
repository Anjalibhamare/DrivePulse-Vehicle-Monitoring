# DrivePulse – Ethernet-Based Vehicle Monitoring System

**Automotive Communication Networks – Ethernet 2 | Linux socket programming in C | TCP + UDP | Wireshark**

DrivePulse models two automotive-network endpoints:
- **Vehicle ECU** (`src/vehicle_ecu.c`): generates periodic vehicle telemetry.
- **Dashboard ECU** (`src/dashboard_ecu.c`): receives, parses and displays telemetry.

Signals include vehicle speed (km/h), engine RPM and steering angle (degrees), plus sequence number, status and timestamp.

## Architecture

```text
Vehicle ECU (C sender) -- Linux IP socket: TCP or UDP --> Dashboard ECU (C receiver)
  speed / RPM / angle                                  parse and display telemetry
```

Both applications can run in separate terminals on one Ubuntu machine. Default endpoint: `127.0.0.1:5005`. This demonstrates application-layer TCP/UDP over loopback; it is not a physical Ethernet PHY, cable, MAC exchange, SOME/IP or 100BASE-T1 implementation.

## Build

```bash
sudo apt update
sudo apt install -y build-essential wireshark
make
```

Binaries are generated in `build/`. Alternatively, run `make -C src` to build the source-folder binaries.

## UDP test

Terminal 1:
```bash
./build/dashboard_ecu --protocol udp --bind 127.0.0.1 --port 5005
```
Terminal 2:
```bash
./build/vehicle_ecu --protocol udp --host 127.0.0.1 --port 5005 --count 20
```

## TCP test

Stop the UDP receiver first. Terminal 1:
```bash
./build/dashboard_ecu --protocol tcp --bind 127.0.0.1 --port 5005
```
Terminal 2:
```bash
./build/vehicle_ecu --protocol tcp --host 127.0.0.1 --port 5005 --count 20
```

The sender transmits approximately one record per second. Use `--count 0` for continuous transmission. TCP uses newline-delimited records and closes its write side when finished.

## Command-line options

Vehicle ECU: `--protocol tcp|udp --host IPv4 [--port N] [--count N] [--interval-ms N]`

Dashboard ECU: `--protocol tcp|udp [--bind IPv4] [--port N]`

## Wireshark analysis

1. Select loopback interface `lo` and start capture.
2. Run UDP and TCP tests separately.
3. Use filters `udp.port == 5005`, `tcp.port == 5005`, `tcp.flags.syn == 1`, `tcp.flags.syn == 1 && tcp.flags.ack == 1`, and `tcp.flags.fin == 1`.
4. Save genuine captures as `wireshark/udp_capture.pcapng` and `wireshark/tcp_capture.pcapng`; add your own screenshots to `screenshots/`.

## Validation scenarios to execute

1. Normal UDP and TCP communication; confirm all three required telemetry values.
2. Receiver stopped: compare UDP send behavior with TCP connection failure.
3. Extended transmission (for example `--count 120`); inspect sequence numbers.
4. Invalid IP input and a valid but unreachable test address (for example `192.0.2.55`); record actual behavior.
5. Compare TCP connection setup, ordered stream and termination with UDP datagrams.

Record your actual observations in the validation report. Do not mark a scenario passed until you have run it locally.

## Repository structure

```text
.
├── README.md
├── Makefile
├── src/
│   ├── vehicle_ecu.c
│   ├── dashboard_ecu.c
│   └── Makefile
├── docs/                 # assignment reports
├── logs/                 # build and smoke-test notes
├── screenshots/          # add genuine screenshots
└── wireshark/             # add genuine PCAP captures
```

## Scope and limitations

- Telemetry is deterministic simulated data, not sensor readings from a real vehicle.
- TCP is a reliable ordered byte stream; UDP is connectionless and does not guarantee delivery or ordering.
- This is an educational Linux socket application, not a production automotive ECU or safety-certified implementation.
- PCAP captures and screenshots must be generated on your own Ubuntu machine; none are fabricated.

## Experiment runner

The project package includes `scripts/run_experiments.sh`, a practical Ubuntu runner intended to compile the programs, exercise TCP/UDP and capture loopback traffic using tcpdump. Install `build-essential` and `tcpdump` first. Review the script before running it and use sudo only for packet-capture permissions. Inspect its outputs and retain genuine captures/logs for your submission.
