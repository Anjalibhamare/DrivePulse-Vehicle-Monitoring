CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -O2
.PHONY: all clean
all: vehicle_ecu dashboard_ecu
vehicle_ecu: vehicle_ecu.c
	$(CC) $(CFLAGS) $< -o $@
dashboard_ecu: dashboard_ecu.c
	$(CC) $(CFLAGS) $< -o $@
clean:
	rm -f vehicle_ecu dashboard_ecu
