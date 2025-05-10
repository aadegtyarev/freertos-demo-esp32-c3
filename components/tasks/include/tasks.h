#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

typedef enum { EVT_BUTTON_PRESS } app_event_t;
extern QueueHandle_t app_queue;

void tasks_start_all(void);
