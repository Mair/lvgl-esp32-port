#include <stdlib.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "process_x.h"

static QueueHandle_t receive_payload_queue;
///////////////External task////////////////
void receive_from_process_x(receive_payload_t *payload)
{
    xQueueReceive(receive_payload_queue, payload, 0);
}
////////////////////////////////

int read_value_of_x(void)
{
    return rand() % 100;
}

void process_x_task(void *params)
{
    receive_payload_queue = xQueueCreate(10, sizeof(receive_payload_t));
    receive_payload_t receive_payload = {};
    while (1)
    {
        receive_payload.value = read_value_of_x();
        xQueueSend(receive_payload_queue, &receive_payload, 0);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void process_x(void)
{
    xTaskCreate(process_x_task, "process_x_task", 1024 * 4, NULL, 5, NULL);
}