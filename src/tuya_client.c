#include <stdio.h>
#include <time.h>
#include "tuyalink_core.h"
#include "tuya_error_code.h"
#include "tuya_log.h"
#include "tuya_cacert.h"
#include "log.h"

#include "tuya_client.h"
#include "config.h"
#include "signal_utils.h"
#include "payload.h"
#include "log_utils.h"


#include <syslog.h>

enum {
    SDK_TRACE = LOG_TRACE,
    SDK_DEBUG = LOG_DEBUG,
    SDK_INFO = LOG_INFO,
    SDK_WARN = LOG_WARN,
    SDK_ERROR = LOG_ERROR,
    SDK_FATAL = LOG_FATAL
};

#undef LOG_DEBUG
#undef LOG_INFO

tuya_mqtt_context_t client_instance;

static int action_save_text(tuyalink_message_t *msg){
    char *text;
    text = convert_tuya_json_to_text(msg->data_string);
    if (text == NULL){
        TY_LOGI("Failed to display action text");
        return;
    }
    FILE *file = fopen("/tmp/tuya_action.log", "a");
    if (file == NULL){
        free_tuya_response_json(text);
        return;
    }
    if (write_to_file(file, text) != 0){
        fclose(file);
        free_tuya_response_json(text);
        return 1;
    }

    TY_LOGI("The message was succesfully saved in /tmp/tuya_action.log");
    fclose(file);
    free_tuya_response_json(text);
    printf("\r\n");
    return 0;
}

static void tuya_syslog_callback_custom(log_Event *event){
    int priority;

    switch (event->level){
        case SDK_TRACE:
        case SDK_DEBUG:
            priority = SDK_DEBUG;
            break;
        case SDK_INFO:
            priority = SDK_INFO;
            break;
        case SDK_WARN:
            priority = LOG_WARN;
            break;
        case SDK_ERROR:
            priority = LOG_ERROR;
            break;
        case SDK_FATAL:
            priority = LOG_FATAL;
            break;
        default:
            priority = LOG_NOTICE;
            break;
    }

    char message[1024];

    vsnprintf(message, sizeof(message), event->fmt, event->ap);

    syslog(priority, "%s:%d: %s", event->file, event->line, message);
}

int logging_tuya(struct arguments *config){
    if (config->daemon != 1){
        log_set_quiet(false);
        log_set_level(SDK_DEBUG);
        return 0;
    }

    if (log_add_callback(tuya_syslog_callback_custom, NULL, LOG_DEBUG) != 0){
        log_error_m("Failed to make logging for Tuya");
        return 1;
    }

    log_set_quiet(true);
    return 0;
}

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

    char *action_code;

    if ((action_code = find_action_tuya(msg->data_string)) == NULL){
        log_error_m("Failed to indentify action code");
        return;
    }

    if (strcmp(action_code, "save_text") == 0){
        if (action_save_text(msg) != 0){
            log_error_m("Failed to perform save_text action");
            goto cleanup_on_messages;
        }
    }

cleanup_on_messages:
    free_action_tuya(action_code);
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
        .on_messages = on_messages
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
