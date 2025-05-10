#include "led.h"
#include "driver/gpio.h"

#define LED_GPIO 2

void led_init(void)
{
    gpio_config_t cfg = {
        .pin_bit_mask = 1ULL<<LED_GPIO,
        .mode         = GPIO_MODE_OUTPUT,
    };
    gpio_config(&cfg);
}

void led_on(void)  { gpio_set_level(LED_GPIO, 1); }
void led_off(void) { gpio_set_level(LED_GPIO, 0); }
