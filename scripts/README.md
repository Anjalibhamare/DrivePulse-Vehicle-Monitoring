# Experiment Runner

`run_experiments.sh` builds the two C applications, starts the dashboard receiver, sends telemetry over TCP and UDP, stores terminal logs, and attempts packet capture when `tcpdump` and capture permissions are available.

Run from the repository root:
```bash
chmod +x scripts/run_experiments.sh
./scripts/run_experiments.sh
```

The capture step requires Ubuntu/Linux, `tcpdump`, and permission to capture on loopback (`lo`). Check the script output and verify the generated PCAP files in Wireshark.
