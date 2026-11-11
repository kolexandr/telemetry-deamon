#ifndef NETWORK_INTERFACE_H
#define NETWORK_INTERFACE_H

#include <ifaddrs.h>
#include <stdint.h>
#include <netinet/in.h>
#include <net/if.h>

struct network_interface{
    char name[IFNAMSIZ];
    char ip[INET6_ADDRSTRLEN];
    char netmask[INET6_ADDRSTRLEN];
    uint64_t transmitted_data_amount;
    uint64_t received_data_amount;
    struct network_interface *next;
};

void print_interfaces(struct network_interface *interface);

int get_network_interfaces(struct network_interface **n_interface);

void free_network_interface(struct network_interface *n_interface);

#endif