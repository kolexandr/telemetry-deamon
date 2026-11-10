#ifndef NETWORK_INTERFACE_H
#define NETWORK_INTERFACE_H

#include <ifaddrs.h>
#include <stdint.h>
#include <netinet/in.h>

struct network_interface{
    char *name;
    int address_family;
    union {
        char ip_address_ip4[INET_ADDRSTRLEN];
        char ip_address_ip6[INET6_ADDRSTRLEN];
    } ip;
    union {
        char ipv4[INET_ADDRSTRLEN];
        char ipv6[INET6_ADDRSTRLEN];
    } netmask;
    uint64_t transmitted_data_amount;
    uint64_t received_data_amount;
    struct network_interface *next;
};

// struct network_interface get_network_interfaces();
int get_network_interfaces();

void free_network_interface(struct network_interface *network_interface);

#endif