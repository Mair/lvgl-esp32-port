#ifndef PROCESS_X_H
#define PROCESS_X_H

typedef struct receive_payload_t
{
    int value;
} receive_payload_t;

void receive_from_process_x(receive_payload_t *payload);

void process_x(void);

#endif