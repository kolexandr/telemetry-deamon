#ifndef SYSTEM_TIME_H
#define SYSTEM_TIME_H

#include <time.h>
#include <sys/sysinfo.h>

struct system_time{
    time_t time;
};

struct system_time get_system_time();

#endif