#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include "tuyalink_core.h"

#include "metrics.h"
#include "payload.h"
#include "tuya_client.h"
#include "config.h"
#include "log_system.h"

int main(int argc, char **argv){
    configure_log();

    struct arguments arguments = {0};
    config_parse(argc, argv, &arguments);
    tuya_mqtt_context_t *client;
    struct metrics metrics = {0};
    char *payload;

    // make error handling
    client = establish_connection(&arguments);
    log_debug("Connection was establish OLEKSANDR");

    if (client == NULL){
        log_critical("Unable to connect to Tuya iot cloud");
        return 1;
    }

    while(1){
        
        if (collect_metrics(&metrics) != 0){
            log_error("Unable to collect metrics");
            continue;
        }

        // print_metrics(&metrics);

        payload = metrics_to_payload(&metrics);

        if (payload == NULL){
            log_error("Failed to create payload");
        }

        log_trace(payload);

        if (payload == NULL){
            log_error("Unable to generate payload");
            continue;
        }

        send_data_tuya(client, payload, &arguments);
        free_payload(payload);
    
        sleep(15);
    }

// cleanup:

    free_metrics(&metrics);
    free_payload(payload);
    close_log();
    return 0;
}

