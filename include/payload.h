#ifndef PAYLOAD_H
#define PAYLOAD_H

#include "metrics.h"
#include <stdio.h>

char *find_action_tuya(char *data);

void free_action_tuya(char *data);

int write_to_file(FILE *file, char *text);

char *convert_tuya_json_to_text(char *data);

void free_tuya_response_json(char *response);

char *metrics_to_payload(struct metrics *metrics);

void free_payload(char *payload);


#endif