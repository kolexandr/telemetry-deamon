#ifndef RAM_MEMORY_H
#define RAM_MEMORY_H

#include <sys/sysinfo.h>
#include <inttypes.h>

struct memory_info {
    uint64_t total_ram;
    uint64_t free_ram;
};

int get_memory_info(struct sysinfo *system_info, struct memory_info *memory_info);

void print_memory_info(struct memory_info memory_info);

#endif