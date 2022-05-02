#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_log.h"

#define core_0 0
#define core_1 1

// Define pin number the default is 13 but it is not recommened to use it one can use another in build LED on LED_1 2
#define LED_1 2
// LED_2 5
#define LED_2 5
// TAG for our logger
static const char *TAG = "LED STATUS";

// xTaskCreatePinnedToCore passes pvParameter although not used here
void blink_led_core_1(void *pvParameter)
{
    // Select GPIO pin
    esp_rom_gpio_pad_select_gpio(LED_1);
    esp_rom_gpio_pad_select_gpio(LED_2);

    // Set the log level to INFO
    esp_log_level_set("LOG", ESP_LOG_INFO);
    // set PIN 2 as output
    gpio_set_direction(LED_1, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_2, GPIO_MODE_OUTPUT);
    // Initialize the signal
    int isON = 0;

    while (true)
    { // Set the output as signal isON
        gpio_set_level(LED_1, isON);
        gpio_set_level(LED_2, isON);
        // A delay of 1 second
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        // LOG the value of signal
        ESP_LOGI(TAG, "Turning the LEDs %s!", isON == true ? "ON" : "OFF");
        // change the value of signal
        isON = !isON;
        // clear (or flush) the output buffer and move the buffered data to console
        fflush(stdout);
    }
}

// xTaskCreatePinnedToCore passes pvParameter although not used here

void hello_task_core_0(void *pvParameter)
{
    while (1)
    { // Print hello world with a delay
        printf("Hello world from core %d!\n", xPortGetCoreID());
        vTaskDelay(1323 / portTICK_PERIOD_MS);
        // clear (or flush) the output buffer and move the buffered data to console
        fflush(stdout);
    }
}

void dual_core_implementation()
{ // init nvs flash storage
    nvs_flash_init();
    // Create task1
    xTaskCreatePinnedToCore(&hello_task_core_0, "core1_task", 1024 * 4, NULL, configMAX_PRIORITIES - 1, NULL, core_0);
    // Create task2
    xTaskCreatePinnedToCore(&blink_led_core_1, "core0_task", 1024 * 4, NULL, configMAX_PRIORITIES - 1, NULL, core_1);
}