#ifndef RAM_MEMORY_H
#define RAM_MEMORY_H

#include <stdio.h>
#include <sys/sysinfo.h>

struct memory_info {
    __uint64_t total_ram;
    __uint64_t free_ram;
};

int get_memory_info(struct sysinfo *system_info, struct memory_info *memory_info);

#endif