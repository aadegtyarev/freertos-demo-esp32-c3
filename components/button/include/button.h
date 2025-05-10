#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "esp_attr.h"

void button_init(void);
void button_isr_handler(void *arg);
void button_debounce_cb(TimerHandle_t xTimer);
