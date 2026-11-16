#!/usr/bin/env bash
# Practical Ubuntu TCP/UDP demonstration and genuine loopback packet capture.
set -Eeuo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"
for tool in gcc make tcpdump; do
  command -v "$tool" >/dev/null || { echo "Missing $tool. Install: sudo apt install build-essential tcpdump" >&2; exit 1; }
done
mkdir -p build wireshark logs
make
DASH_PID=""
CAP_PID=""
cleanup() {
  if [[ -n "$CAP_PID" ]]; then sudo kill -INT "$CAP_PID" 2>/dev/null || true; wait "$CAP_PID" 2>/dev/null || true; fi
  if [[ -n "$DASH_PID" ]]; then kill "$DASH_PID" 2>/dev/null || true; wait "$DASH_PID" 2>/dev/null || true; fi
}
trap cleanup EXIT INT TERM
run_case() {
  local mode="$1" cap="$2"
  echo "[RUN] $mode"
  ./build/dashboard_ecu --protocol "$mode" --bind 127.0.0.1 --port 5005 >"logs/${mode}_dashboard.txt" 2>&1 &
  DASH_PID=$!
  sleep 1
  sudo tcpdump -i lo -nn -s 0 -U -w "$cap" "port 5005" >/dev/null 2>&1 &
  CAP_PID=$!
  sleep 1
  ./build/vehicle_ecu --protocol "$mode" --host 127.0.0.1 --port 5005 --count 8 >"logs/${mode}_vehicle_sender.txt" 2>&1
  sleep 1
  sudo kill -INT "$CAP_PID" 2>/dev/null || true
  wait "$CAP_PID" 2>/dev/null || true
  CAP_PID=""
  kill "$DASH_PID" 2>/dev/null || true
  wait "$DASH_PID" 2>/dev/null || true
  DASH_PID=""
}
run_case udp wireshark/udp_capture.pcap
run_case tcp wireshark/tcp_capture.pcap
echo "Captures saved. Open them in Wireshark; save screenshots of packet details yourself."
