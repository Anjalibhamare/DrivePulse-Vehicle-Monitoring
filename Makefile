CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -O2
.PHONY: all clean
all: build/vehicle_ecu build/dashboard_ecu
build:
	mkdir -p build
build/vehicle_ecu: src/vehicle_ecu.c | build
	$(CC) $(CFLAGS) $< -o $@
build/dashboard_ecu: src/dashboard_ecu.c | build
	$(CC) $(CFLAGS) $< -o $@
clean:
	rm -rf build
