#include <sys/sysinfo.h>
#include <stdio.h>
#include <inttypes.h>
#include "ram_memory.h"

int get_memory_info(struct sysinfo *system_info, struct memory_info *memory_info){
    if (system_info == NULL || memory_info == NULL){
        return -1;
    }
    
    uint64_t total_bytes = (uint64_t)system_info->totalram * system_info->mem_unit;

    uint64_t free_bytes = (uint64_t)system_info->freeram * system_info->mem_unit;

    memory_info->total_ram = total_bytes / (1024ULL * 1024UL);
    memory_info->free_ram = free_bytes / (1024ULL * 1024UL);

    return 0;
}

void print_memory_info(struct memory_info memory_info){
    printf("Total RAM: %" PRId64 "\n", memory_info.total_ram);
    printf("Free RAM: %" PRId64 "\n\n", memory_info.free_ram);
}