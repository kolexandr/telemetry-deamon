## For cloning 

```
git clone --recurse-submodules [LINK]
```

## Files responsibility

main.c - the main point of the program, distributor

config.c - parses argp arguments and returns config

daemon.c - turns into daemon mode if enabled, deattaches terminal, changes file descriptors,      changes directory

tuya_client.c - configures tuya init, mqtt layer, logging into syslog, if daemon enabled

log_utils.c - utils for logging, syslog implementation

signal_utils.c - configuration for signals

metrics.c - main file for collecting all metrics, evokes all other files, calls sysinfo()

system_time.c - collects system time through taking sysinfo struct

ram_memory.c - collects ram memory (total, free) through taking sysinfo struct

cpu_usage.c - collecting cpu usage from /proc/stat

network_interfaces.c - collecting network interfaces through getifaddrrs system call

payload.c - cJSON implementation, generation and freeing payload for transfer

tuya-iot-core-sdkk - external library for tuya

## Setup

For compiling
```
make
```

For cleaning

```
make clean
```

## Help

```
./monitor --help
```