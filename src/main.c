/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

static const char *TAG = "ESP32_IDF_test";

#define LED_PIN GPIO_NUM_5

/* Use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO CONFIG_BLINK_GPIO

#ifdef CONFIG_BLINK_LED_RMT
static led_strip_t *pStrip_a;
int nCount = 0;

void blink_led(bool fOn)
{
    /* If the addressable LED is enabled */
    if (fOn) {
        switch (nCount)
        {
        case 0:
            pStrip_a->set_pixel(pStrip_a, 0, 10, 0, 0);
            nCount++;
            break;
        case 1:
            pStrip_a->set_pixel(pStrip_a, 0, 0, 10, 0);
            nCount++;
            break;
        case 2:
            pStrip_a->set_pixel(pStrip_a, 0, 0, 0, 10);
            nCount++;
            break;
        case 3:
            nCount = 0;
            break;        
        }
        /* Set the LED pixel using RGB from 0 (0%) to 255 (100%) for each color */
        // pStrip_a->set_pixel(pStrip_a, 0, 10, 0, 0);
        /* Refresh the strip to send data */
        pStrip_a->refresh(pStrip_a, 100);
    } else {
        /* Set all LED off to clear all pixels */
        pStrip_a->clear(pStrip_a, 100);
    }
}

static void configure_led(void)
{
    printf("Example configured to blink addressable LED!\n");
    fflush(stdout);
    /* LED strip initialization with the GPIO and pixels number*/
    pStrip_a = led_strip_init(CONFIG_BLINK_LED_RMT_CHANNEL, BLINK_GPIO, 1);
    /* Set all LED off to clear all pixels */
    pStrip_a->clear(pStrip_a, 10);
}

#elif CONFIG_BLINK_LED_GPIO

static void blink_led(void)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
    gpio_set_level(BLINK_GPIO, s_led_state);
}

static void configure_led(void)
{
    ESP_LOGI(TAG, "Example configured to blink GPIO LED!");
    gpio_reset_pin(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
}

#endif

void app_main(void)
{
    // uint8_t fLedState = 0;
    /* Configure the peripheral according to the LED type */
    // configure_led();
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);    
   /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), WiFi%s%s, ",
            CONFIG_IDF_TARGET,
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());

    // blink_led(0);
    while (1) {
        // ESP_LOGI(TAG, "Hello World!");
        // printf("Hello World!\n");
        gpio_set_level(LED_PIN, 1);
        // blink_led(1);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        gpio_set_level(LED_PIN, 0);
        // blink_led(0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
