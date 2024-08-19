#ifndef RAK3172_H
#define RAK3172_H

#define UART_RAK3172_NUM UART_NUM_1
#define RAK3172_TXD_PIN (GPIO_NUM_0)
#define RAK3172_RXD_PIN (GPIO_NUM_1)

#define BUF_SIZE 1024

void init_RAK3172(void);
void RAK3172_sendCommand(const char* command);
void uartRAK3172_receiveTask(void *pvParameters);

#endif