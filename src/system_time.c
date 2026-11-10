#include <sys/sysinfo.h>
#include <system_time.h>
#include <stdio.h>

int get_system_time(struct sysinfo *system_info, struct system_time *system_time){
    system_time->time = system_info->uptime;
    return 0;
}