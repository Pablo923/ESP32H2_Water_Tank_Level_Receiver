#ifndef WS2812B_H
#define WS2812B_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/rmt_tx.h"
#include "driver/gpio.h"
#include "esp_log.h"

typedef struct {
    rmt_encoder_t base;
    rmt_encoder_t *bytes_encoder;
    int state;
} rmt_led_strip_encoder_t;

void init_led(void);
void set_pixel_color(uint8_t r, uint8_t g, uint8_t b);

size_t rmt_encode_led_strip(rmt_encoder_t *encoder, rmt_channel_handle_t channel, const void *primary_data, size_t data_size, rmt_encode_state_t *ret_state);
esp_err_t rmt_del_led_strip_encoder(rmt_encoder_t *encoder);
esp_err_t rmt_led_strip_encoder_reset(rmt_encoder_t *encoder);
esp_err_t rmt_new_led_strip_encoder(rmt_encoder_handle_t *ret_encoder);

void ToggleLED_Red(void);
void ToggleLED_Green(void);
void ToggleLED_Blue(void);

#endif