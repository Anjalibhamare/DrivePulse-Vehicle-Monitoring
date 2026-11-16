# Validation Report

## Environment
Linux, GCC, C11, `-Wall -Wextra -Wpedantic -O2`, IPv4 loopback.

## Executed application checks
| Test | Result |
|---|---|
| Compile both applications | PASS |
| UDP sender to dashboard – 5 records | PASS: 5 received |
| TCP sender to dashboard – 5 records | PASS: 5 received |
| UDP receiver unavailable | Expected: sender may return success; UDP does not confirm delivery |
| TCP receiver unavailable | Expected: connection refused / non-zero exit |
| Invalid IPv4 address | PASS: rejected |

Detailed logs are retained under `logs/` and `results/`.

## Packet evidence status
Genuine packet captures and Wireshark screenshots were not produced in this runtime because raw packet capture was unavailable. Do not interpret the terminal preview as a Wireshark screenshot. On Ubuntu, install tcpdump/Wireshark, run `scripts/run_experiments.sh`, inspect the resulting PCAPs and save actual Wireshark screenshots.
