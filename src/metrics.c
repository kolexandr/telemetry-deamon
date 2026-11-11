#include <stdio.h>
#include <sys/sysinfo.h>
#include <inttypes.h>
#include "metrics.h"

int collect_metrics(struct metrics *metrics){
    if (metrics == NULL){
        fprintf(stderr, "Error: null pointer metrics passed\n");
        return 1;
    }
    
    struct sysinfo system_info;
    
    if (sysinfo(&system_info) == -1){
        perror("Error:");
        return 1;
    }

    if(get_system_time(&system_info, &metrics->uptime) != 0){
        fprintf(stderr, "Error: failed to collect system time\n");
    }
    if (get_memory_info(&system_info, &metrics->ram_memory) != 0){
        fprintf(stderr, "Error: failed to collect memory info\n");
    }
    if (get_network_interfaces(&metrics->interfaces) != 0){
        fprintf(stderr, "Error: failed to collect network interfaces\n");
    }
    if (get_cpu_info(&metrics->cpu_time) != 0){
        fprintf(stderr, "Error: failed to collect cpu times\n");
    }

    
    return 0;
}

void print_metrics(struct metrics *metrics){
    if (metrics == NULL){
        return;
    }

    print_system_time(metrics->uptime);
    print_memory_info(metrics->ram_memory);
    print_interfaces(metrics->interfaces);
    print_cpu_info(&metrics->cpu_time);

}

void free_metrics(struct metrics *metrics){
    free_network_interface(metrics->interfaces);
}