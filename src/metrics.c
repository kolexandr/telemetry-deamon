#include <stdio.h>
#include <sys/sysinfo.h>
#include <log_utils.h>

#include "metrics.h"

int collect_metrics(struct metrics *metrics){
    if (metrics == NULL){
        log_error("Null pointer metrics passed");
        return -1;
    }
    
    struct sysinfo system_info;
    
    if (sysinfo(&system_info) == -1){
        log_error("Failed to read system info");
        return 1;
    }

    if (get_cpu_info(&metrics->cpu_usage) != 0){
        log_error("Failed to collect cpu times\n");
        return 1;
    }
    log_trace("CPU usage was collected succesfully");
    if(get_system_time(&system_info, &metrics->uptime) != 0){
        log_error("Failed to collect system time");
        return 1;
    }
    log_trace("System time was collected succesfully");
    if (get_memory_info(&system_info, &metrics->ram_memory) != 0){
        log_error("Failed to collect ram memory");
        return 1;
    }
    log_trace("Memory info was collected succesfully");
    if (get_network_interfaces(&metrics->interfaces) != 0){
        log_error("Failed to collect network interfaces");
        return 1;
    }
    log_trace("Network interfaces was collected succesfully");
    return 0;
}

void free_metrics(struct metrics *metrics){
    if (metrics == NULL){
        return;
    }
    log_debug("Metrics are freed");
    free_network_interface(metrics->interfaces);
    metrics->interfaces = NULL;
}