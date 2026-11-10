#include <stdio.h>
#include <getopt.h>
#include "collector.h"
#include "network_interface.h"
#include "cpu_usage.h"

// int argc, char **argv
int main(void){
    collect_metrics();
    get_network_interfaces();
    handle_cpu_info();
    return 0;
}

