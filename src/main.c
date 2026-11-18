#include <stdio.h>
#include <unistd.h>
#include "tuyalink_core.h"

#include "metrics.h"
#include "payload.h"
#include "tuya_client.h"
#include "config.h"
#include "log_utils.h"
#include "signal_utils.h"
#include "daemon.h"

int main(int argc, char **argv){
    struct arguments arguments = {0};
    tuya_mqtt_context_t *client;
    struct metrics metrics = {0};
    char *payload;

    configure_log();
    configure_signals();
    config_parse(argc, argv, &arguments);


    if (arguments.daemon == 1){
        daemonize(&arguments);
    }

    client = establish_connection(&arguments);

    if (client == NULL){
        log_critical_m("Unable to connect to Tuya iot cloud");
        return 1;
    }


    log_info_m("Successfully connected to Tuya server");

    while(!is_interrupted()){
        
        if (collect_metrics(&metrics) != 0){
            log_error_m("Unable to collect metrics");
            free_metrics(&metrics);
            wait_tuya(client, 15);
            continue;
        }

        payload = metrics_to_payload(&metrics);

        if (payload != NULL){
            log_debug_m(payload);
            send_data_tuya(client, payload, &arguments);
        } else {
            log_error_m("Failed to generate payload");
        }
        
        free_payload(payload);
        free_metrics(&metrics);

        wait_tuya(client, 15);
    }

    log_info_m("The system shut down, safe exit");
    disconnect_tuya(client);
    log_info_m("Connection with Tuya server has ended");

    close_log();
    return 0;
}

