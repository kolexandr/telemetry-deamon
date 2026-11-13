#ifndef METRICS_H
#define METRICS_H

#include "system_time.h"
#include "network_interface.h"
#include "ram_memory.h"
#include "cpu_usage.h"

struct metrics {
    struct memory_info ram_memory;
    struct system_time uptime;
    struct network_interface *interfaces;
    struct cpu_usage cpu_usage;
};

int collect_metrics(struct metrics *metrics);

void print_metrics(struct metrics *metrics);

void free_metrics(struct metrics *metrics);

#endif