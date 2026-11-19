#include <stdio.h>
#include <cjson/cJSON.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "metrics.h"
#include "log_utils.h"

static char *get_string_from_json(cJSON *text){
    char *response; 
    if (cJSON_IsString(text) && text->valuestring != NULL){
        size_t size = strlen(text->valuestring) + 1; 
        if ((response = malloc(size)) != NULL){
            memcpy(response, text->valuestring, size);
            return response;
        }
    }
    return NULL;
}

char *find_action_tuya(char *data){
    cJSON *json = cJSON_Parse(data);

    if (json == NULL){
        log_error_m("The object is not json | Action tuya");
        return -1;
    }

    cJSON *action = cJSON_GetObjectItemCaseSensitive(json, "actionCode");

    char *response;
    if ((response = get_string_from_json(action)) == NULL){
        log_error_m("Failed to parse string from json");
        cJSON_Delete(json);
        return 1;
    }


    cJSON_Delete(json);
    return response;
}

void free_action_tuya(char *data){
    if (data != NULL){
        free(data);
    }
}

int write_to_file(FILE *file, char *text){
    if (file == NULL || text == NULL){
        log_error_m("Invalid arguments for writing to file");
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
        log_error_m("The object is not json | Convert json to text");
        return NULL;
    }

    cJSON *params = cJSON_GetObjectItemCaseSensitive(json, "inputParams");
    cJSON *text = cJSON_GetObjectItemCaseSensitive(params, "text");

    char *response;
    if ((response = get_string_from_json(text)) == NULL){
        log_error_m("Failed to parse string from json");
        cJSON_Delete(json);
        return 1;
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
        log_error_m("Invalid input received for parameter 'metrics'\n");
        return NULL;
    }
    cJSON *root = cJSON_CreateObject();
    if (root == NULL){
        log_error_m("Failed to create root JSON'\n");
        return NULL;
    }

    cJSON_AddNumberToObject(root, "uptime", (double)metrics->uptime.time);

    cJSON_AddNumberToObject(root, "total_ram_mib", (double)metrics->ram_memory.total_ram);

    cJSON_AddNumberToObject(root, "free_ram_mib", (double)metrics->ram_memory.free_ram);

    cJSON_AddNumberToObject(root, "cpu_usage", metrics->cpu_usage.cpu_percentage);

    cJSON *network_interfaces = cJSON_AddArrayToObject(root, "network_interfaces");

    if (network_interfaces == NULL){
        log_error_m("Failed to create 'network_interfaces' in JSON\n");
        goto error;
    }

    for (struct network_interface *current = metrics->interfaces; current != NULL; current = current->next){
        cJSON *interface = cJSON_CreateObject();
        if (interface == NULL){
            log_error_m("Failed to create 'interface' in JSON\n");
            goto error;
        }
        cJSON_AddStringToObject(interface, "name", current->name);
        cJSON_AddStringToObject(interface, "ip", current->ip);
        cJSON_AddStringToObject(interface, "netmask", current->netmask);
        cJSON_AddNumberToObject(interface, "received_data_amount", (double)current->received_data_amount);
        cJSON_AddNumberToObject(interface, "transmitted_data_amount", (double)current->transmitted_data_amount);
    
        char *string_json = cJSON_PrintUnformatted(interface);
        cJSON_Delete(interface);
        if (string_json == NULL){
            goto error;
        }

        cJSON_AddItemToArray(network_interfaces, cJSON_CreateString(string_json));

        free(string_json);
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
    log_debug_m("Payload was freed");
}

