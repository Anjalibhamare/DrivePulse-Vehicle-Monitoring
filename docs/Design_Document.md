# Vehicle Monitoring System – Design Document

## 1. Overview
Two independent C programs model a Vehicle ECU and a Dashboard ECU. The sender generates repeatable vehicle telemetry and sends it over either TCP or UDP. The receiver parses each record and displays the vehicle parameters.

## 2. Architecture
```text
Vehicle ECU (vehicle_ecu.c)
  speed | engine RPM | steering | sequence | status | timestamp
                    |
          IPv4 socket, port 5005
              TCP or UDP
                    |
Dashboard ECU (dashboard_ecu.c)
  receive -> frame (TCP) / datagram (UDP) -> parse -> display
```
Both applications can run on one Linux host using 127.0.0.1.

## 3. Modules
- **Vehicle ECU:** generates simulated signals and transmits telemetry records.
- **Dashboard ECU:** binds to the selected address and port, receives records, parses fields and displays them.
- **TCP transport:** connection-oriented byte stream; application records are separated by newline delimiters.
- **UDP transport:** connectionless datagrams; each transmitted record is one datagram.

## 4. Data format
`seq=1,speed_kmph=41,engine_rpm=1167,steering_deg=-43,status=NORMAL,timestamp_ms=...`

## 5. Build and run
Run `make`, then start the dashboard receiver and vehicle sender in separate terminals. Refer to the root README for exact TCP and UDP commands.

## 6. Limitations
Signals are simulated, not measured from a physical vehicle. The project demonstrates application-layer socket communication over IPv4 loopback, not physical Ethernet, 100BASE-T1, CAN, SOME/IP, AUTOSAR or a production ECU.
