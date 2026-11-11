#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include "metrics.h"
#include "payload.h"

// int argc, char **argv
int main(void){
    struct metrics metrics = {0};
    char *payload;
    while(1){
        collect_metrics(&metrics);
        print_metrics(&metrics);
        payload = metrics_to_payload(&metrics);
        print_payload(payload);

        sleep(3);
        
    }

    free_metrics(&metrics);
    free_payload(payload);
    return 0;
}

