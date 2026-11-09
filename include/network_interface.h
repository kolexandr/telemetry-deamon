#ifndef NETWORK_INTERFACE_H
#define NETWORK_INTERFACE_H

struct network_interface{
    char name[100];
    char ip_address[100];
    char netmask[100];
    size_t transmitted_data_amount;
    size_t received_data_amount;
};

struct network_interface get_network_interface();

#endif