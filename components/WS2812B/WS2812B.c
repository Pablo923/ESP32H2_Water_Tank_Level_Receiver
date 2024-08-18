#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/rmt_tx.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "WS2812B.h"

#define LED_COUNT 1
#define RMT_GPIO_NUM GPIO_NUM_8

#define RMT_LED_STRIP_RESOLUTION_HZ 10000000 // 10MHz resolution
#define RMT_LED_STRIP_DATA_CHANNEL  0

static rmt_channel_handle_t led_chan = NULL;
static rmt_encoder_handle_t led_encoder = NULL;

// WS2812B timing
#define T0H 350  // 0 bit high time (ns)
#define T1H 900  // 1 bit high time (ns)
#define TL  900  // low time for either bit (ns)

void init_led(void) {
    rmt_tx_channel_config_t tx_chan_config = {
        .gpio_num = RMT_GPIO_NUM,
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = RMT_LED_STRIP_RESOLUTION_HZ,
        .mem_block_symbols = 64,
        .trans_queue_depth = 4,
    };
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &led_chan));
    ESP_ERROR_CHECK(rmt_new_led_strip_encoder(&led_encoder));
    ESP_ERROR_CHECK(rmt_enable(led_chan));
}

void set_pixel_color(uint8_t r, uint8_t g, uint8_t b) {
    uint32_t pixel = (g << 16) | (r << 8) | b; // GRB format for WS2812B
    rmt_transmit_config_t tx_config = {
        .loop_count = 0,
    };
    ESP_ERROR_CHECK(rmt_transmit(led_chan, led_encoder, &pixel, sizeof(uint32_t), &tx_config));
    ESP_ERROR_CHECK(rmt_tx_wait_all_done(led_chan, portMAX_DELAY));
}

void ToggleLED_Red(void)
{
    // Turn on LED
    set_pixel_color(255, 0, 0);
    vTaskDelay(10 / portTICK_PERIOD_MS);
    // Turn off LED
    set_pixel_color(0, 0, 0);
}

void ToggleLED_Blue(void)
{
    // Turn on LED
    set_pixel_color(0, 255, 0);
    vTaskDelay(10 / portTICK_PERIOD_MS);
    // Turn off LED
    set_pixel_color(0, 0, 0);
}

void ToggleLED_Green(void)
{
    // Turn on LED
    set_pixel_color(0, 0, 255);
    vTaskDelay(10 / portTICK_PERIOD_MS);
    // Turn off LED
    set_pixel_color(0, 0, 0);
}

// Internal Functions

size_t rmt_encode_led_strip(rmt_encoder_t *encoder, rmt_channel_handle_t channel, const void *primary_data, size_t data_size, rmt_encode_state_t *ret_state) {
    rmt_led_strip_encoder_t *led_encoder = __containerof(encoder, rmt_led_strip_encoder_t, base);
    rmt_encoder_handle_t bytes_encoder = led_encoder->bytes_encoder;
    rmt_encode_state_t session_state = 0;
    size_t encoded_symbols = 0;
    
    encoded_symbols += bytes_encoder->encode(bytes_encoder, channel, primary_data, data_size, &session_state);
    *ret_state = session_state;
    return encoded_symbols;
}

esp_err_t rmt_del_led_strip_encoder(rmt_encoder_t *encoder) {
    rmt_led_strip_encoder_t *led_encoder = __containerof(encoder, rmt_led_strip_encoder_t, base);
    rmt_del_encoder(led_encoder->bytes_encoder);
    free(led_encoder);
    return ESP_OK;
}

esp_err_t rmt_led_strip_encoder_reset(rmt_encoder_t *encoder) {
    rmt_led_strip_encoder_t *led_encoder = __containerof(encoder, rmt_led_strip_encoder_t, base);
    rmt_encoder_reset(led_encoder->bytes_encoder);
    led_encoder->state = 0;
    return ESP_OK;
}

esp_err_t rmt_new_led_strip_encoder(rmt_encoder_handle_t *ret_encoder) {
    rmt_led_strip_encoder_t *led_encoder = calloc(1, sizeof(rmt_led_strip_encoder_t));
    led_encoder->base.encode = rmt_encode_led_strip;
    led_encoder->base.del = rmt_del_led_strip_encoder;
    led_encoder->base.reset = rmt_led_strip_encoder_reset;
    
    rmt_bytes_encoder_config_t bytes_encoder_config = {
        .bit0 = {
            .level0 = 1,
            .duration0 = T0H / (1e9 / RMT_LED_STRIP_RESOLUTION_HZ),
            .level1 = 0,
            .duration1 = TL / (1e9 / RMT_LED_STRIP_RESOLUTION_HZ),
        },
        .bit1 = {
            .level0 = 1,
            .duration0 = T1H / (1e9 / RMT_LED_STRIP_RESOLUTION_HZ),
            .level1 = 0,
            .duration1 = TL / (1e9 / RMT_LED_STRIP_RESOLUTION_HZ),
        },
        .flags.msb_first = 1,
    };
    ESP_ERROR_CHECK(rmt_new_bytes_encoder(&bytes_encoder_config, &led_encoder->bytes_encoder));
    *ret_encoder = &led_encoder->base;
    return ESP_OK;
}