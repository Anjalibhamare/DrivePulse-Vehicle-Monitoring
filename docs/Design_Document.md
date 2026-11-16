# DrivePulse – Design Document

## Objective
Implement a Linux C socket demonstration of vehicle telemetry exchanged between a Vehicle ECU sender and a Dashboard ECU receiver using TCP and UDP.

## Functional blocks
- Vehicle ECU: generates repeatable simulated speed, RPM and steering-angle values, with sequence, status and timestamp fields.
- Transport: selectable TCP stream or UDP datagrams over IPv4.
- Dashboard ECU: binds to a configured address/port, receives payloads, parses key/value fields and displays the values.

## Data format
`seq=N,speed_kmph=N,engine_rpm=N,steering_deg=N,status=TEXT,timestamp_ms=N\n`

## Default configuration
IPv4 loopback address 127.0.0.1, port 5005, one telemetry message per second.

## TCP and UDP
TCP establishes a connection and provides an ordered byte stream; newline delimiters reconstruct application records. UDP is connectionless and preserves message datagrams but does not guarantee delivery or ordering.

## Limitations
Signals are simulated, not read from a real vehicle. This is an educational socket project, not a production automotive ECU, SOME/IP implementation, physical 100BASE-T1 link, or safety-certified system.
