#include <sys/types.h>
#include <ifaddrs.h>
#include <stdio.h> 
#include <stdlib.h>
#include <net/if.h>
#include <arpa/inet.h>
#include "network_interface.h"
// struct network_interface **network_interface
int get_network_interfaces(void){
    struct ifaddrs *network_interfaces = NULL;
    if (getifaddrs(&network_interfaces) == -1){
        perror("Ifaddrs error ");
        return -1;
    }

    // network_interface = malloc(sizeof(struct network_interface));
    // if(network_interface == NULL){
    //     fprintf(stderr, "Error: failed to allocate the network interface on the heap\n");
    //     return 1;
    // }

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

        char ip_address[INET6_ADDRSTRLEN];
        char netmask[INET6_ADDRSTRLEN];

        const void *ip;
        const void *mask;

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

        if ((inet_ntop(family, ip, ip_address, sizeof(ip_address))) == NULL || (inet_ntop(family, mask, netmask, sizeof(netmask))) == NULL){
            perror("Inet_top error ");
            goto error;
        }

        printf("Inteface name: %s\n", next_addr->ifa_name);
        printf("Ip address: %s\n", ip_address);
        printf("Net mask: %s\n\n", netmask);
    }

    freeifaddrs(network_interfaces);
    return 0;

error:
    freeifaddrs(network_interfaces);
    // free_network_interface();
    return -1;
    
}

void free_network_interface(struct network_interface *network_interface){
    struct network_interface *temp;
    while(network_interface != NULL){
        temp = network_interface->next;
        free(network_interface);
        network_interface = temp;
    }
    network_interface = NULL;
}

// static uint64_t get_network_counter(const char *interface, const char *counter){
//     return 0;
// }