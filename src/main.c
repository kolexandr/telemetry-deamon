#include <stdio.h>
#include <getopt.h>
#include "collector.h"
#include "network_interface.h"

// int argc, char **argv
int main(void){
    collect_metrics();
    get_network_interfaces();
    return 0;
}

