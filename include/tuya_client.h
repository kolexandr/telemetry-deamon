#ifndef TUYA_CLIENT_H
#define TUYA_CLIENT_H

#include "tuyalink_core.h"
#include "config.h"

int send_data_tuya(tuya_mqtt_context_t *client, char *payload, struct arguments *arguments);

tuya_mqtt_context_t *establish_connection(struct arguments *arguments);

#endif