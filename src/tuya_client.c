#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "tuyalink_core.h"
#include "tuya_error_code.h"
#include "tuya_log.h"
#include "tuya_cacert.h"

#include "tuya_client.h"
#include "config.h"
#include "signal_utils.h"
#include "payload.h"


tuya_mqtt_context_t client_instance;

void wait_tuya(tuya_mqtt_context_t *client, unsigned int seconds){
    struct timespec start, now;
    clock_gettime(CLOCK_MONOTONIC, &start);

    while (!is_interrupted()){
        tuya_mqtt_loop(client);

        clock_gettime(CLOCK_MONOTONIC, &now);
        double elapsed = (now.tv_sec - start.tv_sec) + (now.tv_nsec - start.tv_nsec) / 1e9;

        if (elapsed >= seconds){
            break;
        }

        struct timespec pause = {.tv_nsec = 100000000};
        nanosleep(&pause, NULL);
    }
}

static void on_connect(){
    TY_LOGI("on connected");
}

static void on_disconnect(){
    TY_LOGI("on disconnect");
}

static void on_messages(tuya_mqtt_context_t *context, void *user_data, const tuyalink_message_t *msg){
    (void) context;
    (void) user_data;
    if (msg == NULL || msg->type != THING_TYPE_ACTION_EXECUTE || msg->data_string == NULL){
        return;
    }

    TY_LOGI("%d %d", msg->type, msg->code);
    char *text;
    text = convert_tuya_json_to_text(msg->data_string);
    if (text == NULL){
        TY_LOGI("Failed to display action text");
        return;
    }
    FILE *file = fopen("/tmp/tuya-action.txt", "a");
    if (file == NULL){
        free_tuya_response_json(text);
        return;
    }
    if (write_to_file(file, text) != 0){
        fclose(file);
        free_tuya_response_json(text);
        TY_LOGI("Failed to write to /tmp/tuya-action.txt");
        return;
    }


    fclose(file);
    free_tuya_response_json(text);
    printf("\r\n");
}

int send_data_tuya(tuya_mqtt_context_t *client, char *payload, struct arguments *arguments){
    if (client == NULL || payload == NULL){
        return -1;
    }
    return tuyalink_thing_property_report(client, arguments->i , payload);
}



tuya_mqtt_context_t *establish_connection(struct arguments *arguments){
    int ret = OPRT_OK;
    tuya_mqtt_context_t *client = &client_instance;

    ret = tuya_mqtt_init(client, &(const tuya_mqtt_config_t){
        .host = "m1.tuyacn.com",
        .port = 8883,
        .cacert = tuya_cacert_pem,
        .cacert_len = sizeof(tuya_cacert_pem),
        .device_id = arguments->i,
        .device_secret = arguments->s,
        .keepalive = 100,
        .timeout_ms = 2000,
        .on_connected = on_connect,
        .on_disconnect = on_disconnect,
        .on_messages = on_messages,
    });

    if (ret != OPRT_OK){
        TY_LOGE("Tuya initialization failed");
        return NULL;
    }

    ret = tuya_mqtt_connect(client);
    
    if (ret != OPRT_OK){
        TY_LOGE("Tuya connection failed");
        tuya_mqtt_deinit(client);
        return NULL;
    }

    return client;
}

void disconnect_tuya(tuya_mqtt_context_t *client){
    tuya_mqtt_disconnect(client);
    tuya_mqtt_deinit(client);
}
