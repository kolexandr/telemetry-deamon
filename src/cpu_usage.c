// check proc/stat file probably

#include <stdio.h>
#include <inttypes.h>
#include "cpu_usage.h"

int get_cpu_info(struct cpu_time *times){
    FILE *file = fopen("/proc/stat", "r");
    char line[500];

    if (file == NULL){
        fprintf(stderr, "Error: failed to open file\n");
        return 1;
    }

    if (fgets(line, sizeof(line), file) == NULL){
        fprintf(stderr, "Error: failed to read a file\n");
        return 1;
    }

    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;

    if ((sscanf(line, "cpu %llu %llu %llu %llu %llu %llu %llu %llu",
        &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal)) != 8){
            fprintf(stderr, "Error: failed to read formatted input\n");
            return 1;
    }

    times->user = (uint64_t) user;
    times->nice = (uint64_t) nice;
    times->system = (uint64_t) system;
    times->idle = (uint64_t) idle;
    times->iowait = (uint64_t) iowait;
    times->irq = (uint64_t) irq;
    times->softirq = (uint64_t) softirq;
    times->steal = (uint64_t) steal;

    fclose(file);
    return 0;
}

// double calculate_usage(struct cpu_time *previous, struct cpu_time *current){

// }

int handle_cpu_info(void){
    struct cpu_time times;
    if (get_cpu_info(&times) != 0){
        fprintf(stderr, "Error: failed to read cpu info\n");
    }
    printf("CPU: %" PRId64 "\n", times.user);
    printf("CPU: %" PRId64 "\n", times.nice);
    printf("CPU: %" PRId64 "\n", times.system);
    printf("CPU: %" PRId64 "\n", times.idle);
    printf("CPU: %" PRId64 "\n", times.iowait);
    printf("CPU: %" PRId64 "\n", times.irq);
    printf("CPU: %" PRId64 "\n", times.softirq);
    printf("CPU: %" PRId64 "\n", times.steal);
    return 0;
}