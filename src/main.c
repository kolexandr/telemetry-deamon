#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include "tuyalink_core.h"

#include "metrics.h"
#include "payload.h"
#include "tuya_client.h"
#include "config.h"
#include "log_utils.h"
#include "signal_utils.h"

int main(int argc, char **argv){
    struct arguments arguments = {0};
    tuya_mqtt_context_t *client;
    struct metrics metrics = {0};
    char *payload;

    configure_log();
    configure_signals();
    config_parse(argc, argv, &arguments);


    if (arguments.daemon == 1){
        daemonize();
    }

    // make error handling
    client = establish_connection(&arguments);

    if (client == NULL){
        log_critical("Unable to connect to Tuya iot cloud");
        return 1;
    }


    log_info("Connected successfully to Tuya server");

    while(!is_interrupted()){
        
        if (collect_metrics(&metrics) != 0){
            log_error("Unable to collect metrics");
            continue;
        }

        // print_metrics(&metrics);

        payload = metrics_to_payload(&metrics);

        if (payload == NULL){
            log_error("Failed to generate payload");
            continue;
        }

        log_debug(payload);

        send_data_tuya(client, payload, &arguments);
        
        free_payload(payload);
        free_metrics(&metrics);

        sleep(15);
    }

    // free_metrics(&metrics);
    // free_payload(payload);
    log_info("The system shut down, safe exit");
    disconnect_tuya(client);
    log_info("Connection with Tuya server was disconnected");

    close_log();
    return 0;
}

