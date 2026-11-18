#include <stdio.h>
#include <sys/sysinfo.h>

#include "metrics.h"
#include "cpu_usage.h"
#include "system_time.h"
#include "ram_memory.h"
#include "network_interface.h"
#include "log_utils.h"

int collect_metrics(struct metrics *metrics){
    if (metrics == NULL){
        log_error_m("Null pointer metrics passed");
        return -1;
    }
    
    struct sysinfo system_info;
    
    if (sysinfo(&system_info) == -1){
        log_error_m("Failed to read system info");
        return 1;
    }

    if (get_cpu_info(&metrics->cpu_usage) != 0){
        log_error_m("Failed to collect cpu times\n");
        return 1;
    }
    log_debug_m("CPU usage was collected succesfully");
    if(get_system_time(&system_info, &metrics->uptime) != 0){
        log_error_m("Failed to collect system time");
        return 1;
    }
    log_debug_m("System time was collected succesfully");
    if (get_memory_info(&system_info, &metrics->ram_memory) != 0){
        log_error_m("Failed to collect ram memory");
        return 1;
    }
    log_debug_m("Memory info was collected succesfully");
    if (get_network_interfaces(&metrics->interfaces) != 0){
        log_error_m("Failed to collect network interfaces");
        return 1;
    }
    log_debug_m("Network interfaces was collected succesfully");
    return 0;
}

void free_metrics(struct metrics *metrics){
    if (metrics == NULL){
        return;
    }
    log_debug_m("Metrics are freed");
    free_network_interface(metrics->interfaces);
    metrics->interfaces = NULL;
}