#include <sys/types.h>
#include <ifaddrs.h>
#include <stdio.h> 
#include <stdlib.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <inttypes.h>
#include "network_interface.h"

static int get_network_counter(const char *interface, const char *counter, uint64_t *value){
    char path[256];

    snprintf(path, sizeof(path), "/sys/class/net/%s/statistics/%s", interface, counter);
    
    FILE *file = fopen(path, "r");
    if (file == NULL){
        fprintf(stderr, "Error: failed to open the file");
        return -1;
    }

    unsigned long long result;

    if(fscanf(file, "%llu", &result) != 1){
        fclose(file);
        return -1;
    }

    *value = (uint64_t) result; 
    fclose(file);

    return 0;
}

void print_interfaces(struct network_interface *interface){
    if (interface == NULL){
        return;
    }

    while(interface != NULL){
        printf("Interface name: %s\n", interface->name);
        printf("Ip address: %s\n", interface->ip);
        printf("Net mask: %s\n\n", interface->netmask);
        printf("Received bytes: %" PRIu64 "\n", interface->received_data_amount);
        printf("Transmitted bytes: %" PRIu64 "\n\n", interface->transmitted_data_amount);
        interface = interface->next;
    }
}

int get_network_interfaces(struct network_interface **n_interface){
    if (n_interface == NULL){
        return -1;
    }
    struct ifaddrs *network_interfaces = NULL;
    if (getifaddrs(&network_interfaces) == -1){
        perror("Ifaddrs error ");
        return -1;
    }
    free_network_interface(*n_interface);
    

    *n_interface = NULL;
    struct network_interface *tail_interface = NULL;

    for (struct ifaddrs *next_addr = network_interfaces; next_addr != NULL; next_addr = next_addr->ifa_next){
        if (next_addr->ifa_addr == NULL || next_addr->ifa_netmask == NULL){
            continue;
        }

        if (next_addr->ifa_flags & IFF_LOOPBACK){
            continue;
        }

        int family = next_addr->ifa_addr->sa_family;
        if (family != AF_INET && family != AF_INET6){
            continue;
        }

        const void *ip = NULL;
        const void *mask = NULL;

        if (family == AF_INET){
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)next_addr->ifa_addr;
            struct sockaddr_in *netmaskv4 = (struct sockaddr_in *)next_addr->ifa_netmask;
            ip = &ipv4->sin_addr;
            mask = &netmaskv4->sin_addr;
        } else if (family == AF_INET6){
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)next_addr->ifa_addr;
            struct sockaddr_in6 *netmaskv6 = (struct sockaddr_in6 *)next_addr->ifa_netmask;
            
            ip = &ipv6->sin6_addr;
            mask = &netmaskv6->sin6_addr;
        }

        struct network_interface *new_interface = calloc(1, sizeof(struct network_interface));

        if (new_interface == NULL){
            fprintf(stderr, "Error: failed to allocate heap memory\n");
            goto error;
        }

        if ((inet_ntop(family, ip, new_interface->ip, sizeof(new_interface->ip))) == NULL || (inet_ntop(family, mask, new_interface->netmask, sizeof(new_interface->netmask))) == NULL){
            perror("Inet_top error ");
            goto error;
        }


        snprintf(new_interface->name, sizeof(new_interface->name), "%s", next_addr->ifa_name);

        if ((get_network_counter(next_addr->ifa_name, "rx_bytes", &new_interface->received_data_amount)) != 0 || (get_network_counter(next_addr->ifa_name, "tx_bytes", &new_interface->transmitted_data_amount)) != 0){
            fprintf(stderr, "Error: failed to read network counter\n");  
            goto error;
        }


        if (*n_interface == NULL){
            *n_interface = new_interface;
        } else {
            tail_interface->next = new_interface;
        }

        tail_interface = new_interface;
    }

    freeifaddrs(network_interfaces);
    return 0;

error:
    freeifaddrs(network_interfaces);
    free_network_interface(*n_interface);
    *n_interface = NULL;
    return -1;
    
}

void free_network_interface(struct network_interface *n_interface){
    if (n_interface == NULL){
        return;
    }
    
    struct network_interface *temp;
    while(n_interface != NULL){
        temp = n_interface->next;
        free(n_interface);
        n_interface = temp;
    }
    n_interface = NULL;
}