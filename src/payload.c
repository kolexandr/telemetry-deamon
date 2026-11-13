#include <stdio.h>
#include <cjson/cJSON.h>
#include <time.h>

#include "metrics.h"
#include "log_system.h"

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

    while (metrics->interfaces != NULL){
        cJSON *interface = cJSON_CreateObject();
        if (interface == NULL){
            log_error("Failed to create 'interface' in JSON\n");
            goto error;
        }
        cJSON_AddStringToObject(interface, "name", metrics->interfaces->name);
        cJSON_AddStringToObject(interface, "ip", metrics->interfaces->ip);
        cJSON_AddStringToObject(interface, "netmask", metrics->interfaces->netmask);
        cJSON_AddNumberToObject(interface, "received_data_amount", (double)metrics->interfaces->received_data_amount);
        cJSON_AddNumberToObject(interface, "transmitted_data_amount", (double)metrics->interfaces->transmitted_data_amount);
    
        cJSON_AddItemToArray(network_interfaces, interface);
        metrics->interfaces = metrics->interfaces->next;
    }
    
    char *payload = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return payload;

error:
    cJSON_Delete(root);
    return NULL;
}

void free_payload(char *payload){
    if (payload != NULL){
        log_trace("Payload was freed");
        cJSON_free(payload);
    }
}

// void print_payload(char *payload){
//     if (payload != NULL){
//         puts(payload);
//     }
// }
