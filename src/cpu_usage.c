#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>
#include <math.h>
#include <log_system.h>
#include <string.h>

#include "cpu_usage.h"


// handle all prints in log in one function
void print_cpu_info(struct cpu_usage *cpu_usage){
    if (cpu_usage == NULL){
        log_error("Invalid input received for parameter 'cpu_usage'\n");
        return;
    }
    // char percentage[20] = sprintf(percentage, "%f", cpu_usage->cpu_percentage);
    // char strncat("Cpu percentage: ", percentage, sizeof(percentage));

    // printf("Cpu percentage: %.2f\n", cpu_usage->cpu_percentage);
    // printf("CPU: %" PRId64 "\n", times->user);
    // printf("CPU: %" PRId64 "\n", times->nice);
    // printf("CPU: %" PRId64 "\n", times->system);
    // printf("CPU: %" PRId64 "\n", times->idle);
    // printf("CPU: %" PRId64 "\n", times->iowait);
    // printf("CPU: %" PRId64 "\n", times->irq);
    // printf("CPU: %" PRId64 "\n", times->softirq);
    // printf("CPU: %" PRId64 "\n", times->steal);
    return;
}

static int read_cpu_info(struct cpu_time *times){
    FILE *file = fopen("/proc/stat", "r");
    char line[500];

    if (file == NULL){
        log_error("Failed to read open file\n");
        fprintf(stderr, "Error: failed to open file\n");
        return 1;
    }

    if (fgets(line, sizeof(line), file) == NULL){
        fprintf(stderr, "Error: failed to read a file\n");
        goto error;
    }

    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;

    if ((sscanf(line, "cpu %llu %llu %llu %llu %llu %llu %llu %llu",
        &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal)) != 8){
            fprintf(stderr, "Error: failed to read formatted input\n");
            goto error;
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

error:
    fclose(file);
    return 1;
}

double calculate_usage(struct cpu_time *previous, struct cpu_time *current, struct cpu_usage *cpu_usage){
    if (previous == NULL || current == NULL || cpu_usage == NULL){
        fprintf(stderr, "Error: invalid arguments\n");
        return -1;
    }

    uint64_t previous_total =
        previous->user + previous->nice + previous->system +
        previous->idle + previous->iowait + previous->irq +
        previous->softirq + previous->steal;

    uint64_t current_total =
        current->user + current->nice + current->system +
        current->idle + current->iowait + current->irq +
        current->softirq + current->steal;

    if (current_total <= previous_total){
        fprintf(stderr, "Error: invalid CPU time difference\n");
        return -1;
    }

    uint64_t total_delta = current_total - previous_total;

    uint64_t idle_delta = (current->idle + current->iowait) - (previous->idle + previous->iowait);

    double cpu_percentage = 100 * (double)(total_delta - idle_delta) / (double) total_delta;
    cpu_percentage = round(cpu_percentage * 100) / 100;
    cpu_usage->cpu_percentage = cpu_percentage;
    return 0;
}

int get_cpu_info(struct cpu_usage *cpu_usage){
    struct cpu_time cpu_time_1, cpu_time_2;
    
    if (read_cpu_info(&cpu_time_1) != 0){
        return 1;
    }

    sleep(1);

    if (read_cpu_info(&cpu_time_2) != 0){
        return 1;
    }

    if (calculate_usage(&cpu_time_1, &cpu_time_2, cpu_usage) != 0){
        return 1;
    }
    return 0;
}
