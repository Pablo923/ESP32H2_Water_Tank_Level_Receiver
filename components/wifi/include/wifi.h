#ifndef WIFI_H
#define WIFI_H

#include "esp_err.h"

esp_err_t wifi_connect(const char* ssid, const char* password);

#endif // WIFI_H