#include "ram_memory.h"
#include "system_time.h"
#include <sys/sysinfo.h>
#include <stdio.h>

// remove it 
// int collect_metrics(void){
//     struct sysinfo system_info = {0};
//     struct memory_info memory_info = {0};
//     struct system_time system_time = {0};

//     if (sysinfo(&system_info) == -1){
//         perror("Error:");
//         return 1;
//     }

//     get_system_time(&system_info, &system_time);
//     get_memory_info(&system_info, &memory_info);

//     printf("System time: %ld\n", system_time.time);
//     printf("Memory_info: %ld %ld\n\n", memory_info.total_ram, memory_info.free_ram);

//     return 0;
// }