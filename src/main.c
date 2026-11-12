#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include "tuyalink_core.h"

#include "metrics.h"
#include "payload.h"
#include "tuya_client.h"

// int argc, char **argv
int main(void){
    tuya_mqtt_context_t *client;
    struct metrics metrics = {0};
    char *payload;

    client = establish_connection();

    if (client == NULL){
        return 1;
    }
    // there should be two collections with interval, to calculate cpu usage. we need to compare first one and second
    while(1){
        
        collect_metrics(&metrics);

        print_metrics(&metrics);

        payload = metrics_to_payload(&metrics);

        if (payload == NULL){
            continue;
        }

        print_payload(payload);
        send_data_tuya(client, payload);
        free_payload(payload);
    
        sleep(5);
    }

    free_metrics(&metrics);
    free_payload(payload);
    return 0;
}

