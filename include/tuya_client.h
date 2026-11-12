#ifndef TUYA_CLIENT_H
#define TUYA_CLIENT_H

#include "tuyalink_core.h"

int send_data_tuya(tuya_mqtt_context_t *client, char *payload);

tuya_mqtt_context_t *establish_connection();

#endif