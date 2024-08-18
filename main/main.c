#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "WS2812B.h"
#include "UART.h"

static const char *TAG = "WATERTANK_RECEIVER";

void app_main(void)
{
    ESP_LOGI(TAG, "Initializing... \r\n");

    init_led();
    init_uart();

    // Create Task to Receive Message by UART
    xTaskCreate(uartSerial_receiveTask, "uartSerial_receiveTask", 2048, NULL, 10, NULL);

    // while (1) 
    // {
    //     // Blinky RGB Led to Green
    //     ToggleLED_Green();
    //     vTaskDelay(pdMS_TO_TICKS(1000));
    // }
}
