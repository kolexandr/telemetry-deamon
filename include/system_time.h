#ifndef SYSTEM_TIME_H
#define SYSTEM_TIME_H

#include <time.h>
#include <sys/sysinfo.h>
#include <stdint.h>

struct system_time {
    // time_t time;
    uint64_t time;
};

int get_system_time(struct sysinfo *system_info, struct system_time *system_time);

#endif