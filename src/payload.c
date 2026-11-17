#include <stdio.h>
#include <cjson/cJSON.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "metrics.h"
#include "log_utils.h"

int write_to_file(FILE *file, char *text){
    if (file == NULL || text == NULL){
        log_error("Invalid arguments for writing to file");
        return -1;
    }

    if (fprintf(file, "%s\n", text) < 0){
        return 1;
    }
    return 0;
}

char *convert_tuya_json_to_text(char *data){
    cJSON *json = cJSON_Parse(data);
    
    if (json == NULL){
        log_error("The object is not json");
        return NULL;
    }

    cJSON *params = cJSON_GetObjectItemCaseSensitive(json, "inputParams");
    cJSON *text = cJSON_GetObjectItemCaseSensitive(params, "text");

    char *response = NULL;
    if (cJSON_IsString(text) && text->valuestring != NULL){
        size_t size = strlen(text->valuestring) + 1; 
        if ((response = malloc(size)) != NULL){
            memcpy(response, text->valuestring, size);
        }
    }

    cJSON_Delete(json);
    return response;
}

void free_tuya_response_json(char *response){
    if (response != NULL){
        free(response);
    }
}

char *metrics_to_payload(struct metrics *metrics){
    if (metrics == NULL){
        log_error("Invalid input received for parameter 'metrics'\n");
        return NULL;
    }
    cJSON *root = cJSON_CreateObject();
    if (root == NULL){
        log_error("Failed to create root JSON'\n");
        return NULL;
    }

    cJSON_AddNumberToObject(root, "Uptime", (double)metrics->uptime.time);

    cJSON_AddNumberToObject(root, "Total_ram_mib", (double)metrics->ram_memory.total_ram);

    cJSON_AddNumberToObject(root, "Free_ram_mib", (double)metrics->ram_memory.free_ram);

    cJSON_AddNumberToObject(root, "Cpu_usage", metrics->cpu_usage.cpu_percentage);

    cJSON *network_interfaces = cJSON_AddArrayToObject(root, "Network_interfaces");

    if (network_interfaces == NULL){
        log_error("Failed to create 'network_interfaces' in JSON\n");
        goto error;
    }

    for (struct network_interface *current = metrics->interfaces; current != NULL; current = current->next){
        cJSON *interface = cJSON_CreateObject();
        if (interface == NULL){
            log_error("Failed to create 'interface' in JSON\n");
            goto error;
        }
        cJSON_AddStringToObject(interface, "name", current->name);
        cJSON_AddStringToObject(interface, "ip", current->ip);
        cJSON_AddStringToObject(interface, "netmask", current->netmask);
        cJSON_AddNumberToObject(interface, "received_data_amount", (double)current->received_data_amount);
        cJSON_AddNumberToObject(interface, "transmitted_data_amount", (double)current->transmitted_data_amount);
    
        cJSON_AddItemToArray(network_interfaces, interface);
    }
    
    char *payload = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return payload;

error:
    cJSON_Delete(root);
    return NULL;
}

void free_payload(char *payload){
    if (payload == NULL){
        return;
    }
    cJSON_free(payload);
    log_trace("Payload was freed");
}

// void print_payload(char *payload){
//     if (payload != NULL){
//         puts(payload);
//     }
// }
