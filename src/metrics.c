#include <stdio.h>
#include <sys/sysinfo.h>
#include <log_system.h>

#include "metrics.h"

int collect_metrics(struct metrics *metrics){
    if (metrics == NULL){
        log_error("Null pointer metrics passed");
        return 1;
    }
    
    struct sysinfo system_info;
    
    if (sysinfo(&system_info) == -1){
        log_error("Failed to read system info");
        return 1;
    }



    // check if we want to continue if some of the metrics was not collected
    if (get_cpu_info(&metrics->cpu_usage) != 0){
        log_error("Failed to collect cpu times\n");
    }
    log_info("CPU usage was collected succesfully");
    if(get_system_time(&system_info, &metrics->uptime) != 0){
        log_error("Failed to collect system time");
    }
    log_info("System time was collected succesfully");
    if (get_memory_info(&system_info, &metrics->ram_memory) != 0){
        log_error("Failed to collect ram memory");
    }
    log_info("Memory info was collected succesfully");
    if (get_network_interfaces(&metrics->interfaces) != 0){
        log_error("Failed to collect network interfaces");
    }
    log_info("Network interfaces was collected succesfully");
    return 0;
}

void print_metrics(struct metrics *metrics){
    if (metrics == NULL){
        log_error("Null pointer metrics passed");
        return;
    }

    // log_trace(system_time_to_string(metrics->uptime));
    print_memory_info(metrics->ram_memory);
    print_interfaces(metrics->interfaces);
    print_cpu_info(&metrics->cpu_usage);

}

void free_metrics(struct metrics *metrics){
    log_info("Metrics are freed");
    free_network_interface(metrics->interfaces);
}