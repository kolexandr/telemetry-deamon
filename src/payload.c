// convert metrics to JSON format
#include <stdio.h>
#include <cjson/cJSON.h>
#include <time.h>
#include "metrics.h"

char *metrics_to_payload(struct metrics *metrics){
    if (metrics == NULL){
        return NULL;
    }
    cJSON *root = cJSON_CreateObject();
    if (root == NULL){
        return NULL;
    }
    cJSON *uptime = cJSON_AddObjectToObject(root, "Uptime");
    if (uptime == NULL){
        cJSON_Delete(root);
        return NULL;
    }
    cJSON_AddNumberToObject(uptime, "value", (double)metrics->uptime.time);
    cJSON_AddNumberToObject(uptime, "time", (double)time(NULL));
    char *payload = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return payload;
}

void free_payload(char *payload){
    if (payload != NULL){
        cJSON_free(payload);
    }
}

void print_payload(char *payload){
    if (payload != NULL){
        puts(payload);
    }
}
