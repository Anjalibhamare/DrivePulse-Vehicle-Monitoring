# Experiment Scripts

## run_experiments.sh

The `run_experiments.sh` script builds DrivePulse and runs the TCP and UDP loopback communication experiments. It records terminal output and validation results in the project folders.

## Run on Ubuntu

From the repository root:

```bash
chmod +x scripts/run_experiments.sh
./scripts/run_experiments.sh
```

For packet capture, install the required tool first:

```bash
sudo apt update
sudo apt install tcpdump
```

Run with the permissions required by your Ubuntu packet-capture configuration. The script can only save PCAP evidence when packet capture is available and permitted.

## Expected experiment checks

- Compile both C applications.
- Run the dashboard receiver and vehicle telemetry sender over TCP.
- Run the dashboard receiver and vehicle telemetry sender over UDP.
- Save experiment logs and validation summaries.
- Attempt packet capture when `tcpdump` is installed and capture permissions are available.

## Notes

The applications use IPv4 loopback (`127.0.0.1`) and port `5005`. These experiments demonstrate application-level TCP/UDP socket communication on Linux; they do not represent a physical automotive Ethernet network or an automotive Ethernet PHY.

Check the script output for the actual result of each run. Do not treat a missing PCAP file as a successful packet capture.
