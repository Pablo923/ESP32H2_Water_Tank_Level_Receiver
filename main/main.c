#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "WS2812B.h"
#include "UART.h"
#include "RAK3172.h"

static const char *TAG = "WATERTANK_RECEIVER";

void app_main(void)
{
    ESP_LOGI(TAG, "Initializing... \r\n");

    init_led();
    init_uart();
    init_RAK3172();

    // Create Task to Receive Message by UART
    xTaskCreate(uartSerial_receiveTask, "uartSerial_receiveTask", 2048, NULL, 10, NULL);
    // Create Task to Print RAK3172 Responses
    xTaskCreate(uartRAK3172_receiveTask, "uartRAK3172_receiveTask", 2048, NULL, 10, NULL);
    
    // Set RAK3172 as Receiver
    RAK3172_sendCommand("AT+PRECV=65534");
}
