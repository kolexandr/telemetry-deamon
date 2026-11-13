// talk to tuya sdk client
#include <assert.h>
#include "tuyalink_core.h"
#include "tuya_error_code.h"
#include "tuya_log.h"
#include "tuya_cacert.h"

#include "tuya_client.h"
#include "config.h"

// accept product ID, device ID, device secret in function
// const char productId[] = "7jvnvlgnik6prmly";
// const char deviceId[] = "262fc26e16c62ad133o6zk";
// const char deviceSecret[] = "xZLvICp28VCANxME";


tuya_mqtt_context_t client_instance;
// tuya_mqtt_context_t *context, void *user_data
static void on_connect(){
    TY_LOGI("on connected");
}

static void on_disconnect(){
    TY_LOGI("on disconnect");
}

static void on_messages(tuya_mqtt_context_t *context, void *user_data, const tuyalink_message_t *msg){
    TY_LOGI("on message id:%s, type:%d, code:%d", msg->msgid, msg->type, msg->code);
    switch(msg->type){
        case THING_TYPE_MODEL_RSP:
            TY_LOGI("Model data:%s", msg->data_string);
            break;
        case THING_TYPE_PROPERTY_SET:
            TY_LOGI("property set:%s", msg->data_string);
            break;
        case THING_TYPE_PROPERTY_REPORT_RSP:
            break;
        default:
            break;
    }
    (void) context;
    (void) user_data;
    // TY_LOGI(, user_data);
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
        .on_messages = on_messages
    });

    // TODO handle SIGABRT, free memories before that 
    assert(ret == OPRT_OK);

    ret = tuya_mqtt_connect(client);
    assert(ret == OPRT_OK);

    // for (;;){
    //     tuya_mqtt_loop(client);
    //     tuyalink_thing_property_report(client, deviceId, payload);
    // }
    return client;
}
