// convert metrics to JSON format
#include <stdio.h>
#include <cjson/cJSON.h>
#include <time.h>
#include "metrics.h"

// Convert every structure and value 
char *metrics_to_payload(struct metrics *metrics){
    if (metrics == NULL){
        return NULL;
    }
    cJSON *root = cJSON_CreateObject();
    if (root == NULL){
        return NULL;
    }
    // cJSON *uptime = cJSON_AddObjectToObject(root, "Uptime");
    // if (uptime == NULL){
    //     goto error;
    // }
    // cJSON *total_ram = cJSON_AddObjectToObject(root, "Total_ram_mib");
    // if (total_ram == NULL){
    //     goto error;
    // }
    // cJSON *free_ram = cJSON_AddObjectToObject(root, "Free_ram_mib");
    // if (free_ram == NULL){
    //     goto error;
    // }
    // cJSON *interfaces = cJSON_AddObjectToObject(root, "Total_ram_mib");
    // if (total_ram == NULL){
    //     goto error;
    // }


    cJSON_AddNumberToObject(root, "Uptime", (double)metrics->uptime.time);
    
    cJSON_AddNumberToObject(root, "Total_ram_mib", (double)metrics->ram_memory.total_ram);

    cJSON_AddNumberToObject(root, "Free_ram_mib", (double)metrics->ram_memory.free_ram);

    // cJSON_AddArrayToObject();


    // cJSON_AddNumberToObject(uptime, "time", (double)time(NULL));

    // cJSON_AddNumberToObject(uptime, "time", (double)time(NULL));
    
    // cJSON_AddNumberToObject(uptime, "time", (double)time(NULL));
    
    // cJSON_AddNumberToObject(uptime, "time", (double)time(NULL));
    
    char *payload = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return payload;

error:
    cJSON_Delete(root);
    return NULL;
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
