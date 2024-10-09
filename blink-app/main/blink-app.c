#include <stdint.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define BLINK_GPIO 2

static const char *TAG = "blink-app";

static void init_gpio(void)
{
    
    gpio_reset_pin(BLINK_GPIO);
    
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << BLINK_GPIO);
    gpio_config(&io_conf);
    gpio_set_level(BLINK_GPIO, 1);

    gpio_dump_io_configuration(stdout, (1ULL << BLINK_GPIO));

    ESP_LOGI(TAG, "Initialized GPIO %d", BLINK_GPIO);
}

static void blink_task(void *pvParameters)
{
    uint8_t gpio_level = 0;

    while (1) {
        gpio_set_level(BLINK_GPIO, gpio_level);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        gpio_level = !gpio_level;
        ESP_LOGI(TAG, "Blinking LED on GPIO %d State %d", BLINK_GPIO, gpio_level);
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "Starting blink-app");
    init_gpio();
    xTaskCreate(&blink_task, "blink_task", 2048, NULL, 5, NULL);
}
