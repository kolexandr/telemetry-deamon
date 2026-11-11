#include <stdio.h>
#include <getopt.h>
#include "metrics.h"

// int argc, char **argv
int main(void){
    struct metrics metrics = {0};
    collect_metrics(&metrics);

    print_metrics(&metrics);

    free_metrics(&metrics);

    return 0;
}

