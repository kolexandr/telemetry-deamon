#ifndef METRICS_H
#define METRICS_H

#include "system_time.h"
#include "network_interface.h"
#include "ram_memory.h"
#include "cpu_usage.h"

struct statistics{
    memory_info ram_memory;
    system_time uptime;
    cpu_usage cpu_usage;
};

int set_metrics(void);

int collect_metrics(void);

#endif