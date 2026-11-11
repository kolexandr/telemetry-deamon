#ifndef PAYLOAD_H
#define PAYLOAD_H

#include "metrics.h"

char *metrics_to_payload(struct metrics *metrics);

void free_payload(char *payload);

void print_payload(char *payload);

#endif