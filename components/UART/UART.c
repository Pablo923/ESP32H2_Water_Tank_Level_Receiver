#include "driver/gpio.h"
#include "driver/uart.h"
#include "string.h"
#include "WS2812B.h"
#include "UART.h"

void init_uart(void) {
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    
    // Init USB Serial Port
    uart_driver_install(UART_SERIAL_NUM, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_SERIAL_NUM, &uart_config);
    uart_set_pin(UART_SERIAL_NUM, SERIAL_TXD_PIN, SERIAL_RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

void SendUART_Message(const char* message) {
    uart_write_bytes(UART_SERIAL_NUM, message, strlen(message));
}

void uartSerial_receiveTask(void *pvParameters) 
{
    static const char *TAG_SERIAL = "[SERIAL] ";

    uint8_t* data = (uint8_t*) malloc(BUF_SIZE);
    
    while (1) 
    {
        int len = uart_read_bytes(UART_SERIAL_NUM, data, BUF_SIZE, 20 / portTICK_PERIOD_MS);
        if (len > 0 )
        {   
            data[len] = 0;  // Null-terminate the received data

            // Blink RGB Led Blue to Notify
            ToggleLED_Blue();

            // Print Received Message
            SendUART_Message(TAG_SERIAL);
            SendUART_Message((char*)data);
        }
    }
    free(data);
    vTaskDelay(pdMS_TO_TICKS(10));
}