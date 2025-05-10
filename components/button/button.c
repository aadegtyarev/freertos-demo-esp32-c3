#include "button.h"
#include "driver/gpio.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "tasks.h"

#define BUTTON_GPIO            1
#define BUTTON_PRESSED_LEVEL   0
#define BUTTON_DEBOUNCE_MS     100

static const char *TAG = "button";
static TimerHandle_t debounce_timer;
extern QueueHandle_t app_queue;

void IRAM_ATTR button_isr_handler(void *arg)
{
    BaseType_t hp = pdFALSE;
    xTimerStartFromISR(debounce_timer, &hp);
    if (hp) portYIELD_FROM_ISR();
}

void button_debounce_cb(TimerHandle_t t)
{
    if (gpio_get_level(BUTTON_GPIO) == BUTTON_PRESSED_LEVEL) {
        app_event_t ev = EVT_BUTTON_PRESS;
        xQueueSend(app_queue, &ev, portMAX_DELAY);
        ESP_LOGI(TAG, "Button press event");
    }
}

void button_init(void)
{
    gpio_config_t cfg = {
        .pin_bit_mask = 1ULL<<BUTTON_GPIO,
        .mode         = GPIO_MODE_INPUT,
        .pull_up_en   = GPIO_PULLUP_ENABLE,
        .intr_type    = GPIO_INTR_NEGEDGE
    };
    gpio_config(&cfg);

    debounce_timer = xTimerCreate(
        "btn_deb",
        pdMS_TO_TICKS(BUTTON_DEBOUNCE_MS),
        pdFALSE, NULL,
        button_debounce_cb
    );

    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_GPIO, button_isr_handler, NULL);
}
