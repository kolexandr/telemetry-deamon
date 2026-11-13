#ifndef CPU_INFO_H
#define CPU_INFO_H

#include <stdint.h>

struct cpu_time{
    uint64_t user;
    uint64_t nice;
    uint64_t system;
    uint64_t idle;
    uint64_t iowait;
    uint64_t irq;
    uint64_t softirq;
    uint64_t steal;
};

struct cpu_usage{
    double cpu_percentage;
};

void print_cpu_info(struct cpu_usage *cpu_usage);

int get_cpu_info(struct cpu_usage *cpu_usage);

#endif