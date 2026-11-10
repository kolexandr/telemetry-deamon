#include <sys/sysinfo.h>
#include "ram_memory.h"

int get_memory_info(struct sysinfo *system_info, struct memory_info *memory_info){
    memory_info->total_ram = system_info->totalram;
    memory_info->free_ram = system_info->freeram;

    return 0;
}