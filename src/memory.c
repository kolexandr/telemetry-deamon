#include <sys/sysinfo.h>
#include <stdio.h>
#include <inttypes.h>
#include "ram_memory.h"

int get_memory_info(struct sysinfo *system_info, struct memory_info *memory_info){
    // change it
    memory_info->total_ram = system_info->totalram / (1024ULL * 1024UL);
    memory_info->free_ram = system_info->freeram / (1024ULL * 1024UL);

    return 0;
}

void print_memory_info(struct memory_info memory_info){
    printf("Total RAM: %" PRId64 "\n", memory_info.total_ram);
    printf("Free RAM: %" PRId64 "\n\n", memory_info.free_ram);
}