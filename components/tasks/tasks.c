#include "tasks.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "led.h"
#include "button.h"

QueueHandle_t app_queue = NULL;
static const char *TAG = "led_task";

static void led_task(void *arg)
{
    app_event_t ev;
    for (;;) {
        if (xQueueReceive(app_queue, &ev, portMAX_DELAY) == pdTRUE) {
            ESP_LOGI(TAG, "Blink start");
            for (int i = 0; i < 5; i++) {
                led_on();
                vTaskDelay(pdMS_TO_TICKS(500));
                led_off();
                vTaskDelay(pdMS_TO_TICKS(500));
            }
            ESP_LOGI(TAG, "Blink done");
        }
    }
}

void tasks_start_all(void)
{
    app_queue = xQueueCreate(10, sizeof(app_event_t));
    led_init();
    button_init();
    xTaskCreate(led_task, "led", 2048, NULL, 5, NULL);
}
