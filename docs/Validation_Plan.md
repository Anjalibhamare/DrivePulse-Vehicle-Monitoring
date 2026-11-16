# DrivePulse – Validation Plan and Report Template

Run and record the following scenarios on your Ubuntu machine. Do not mark a test as passed until you have executed it and retained evidence.

| # | Scenario | Procedure | Evidence to record |
|---|---|---|---|
| 1 | Normal communication | Run dashboard and sender in UDP, then TCP | Received speed, RPM, steering angle and packet capture |
| 2 | Receiver unavailable | Stop dashboard; run sender in both modes | Actual UDP behavior and TCP connection error |
| 3 | Extended transmission | Run sender with --count 120 | Sequence numbers, missing/duplicate records, duration |
| 4 | Invalid/unreachable IP | Try --host not-an-ip, then 192.0.2.55 | Validation error and actual network behavior |
| 5 | TCP vs UDP | Capture both modes in Wireshark | Handshake, payload, termination and datagram observations |

## Result record
- Test date / Ubuntu version:
- GCC version:
- Wireshark version:
- Test 1 observations:
- Test 2 observations:
- Test 3 observations:
- Test 4 observations:
- Test 5 observations:
- PCAP filenames:
- Screenshot filenames:
