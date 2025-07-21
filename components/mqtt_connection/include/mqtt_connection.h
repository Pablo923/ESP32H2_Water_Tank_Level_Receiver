#ifndef MQTT_CONNECTION_H
#define MQTT_CONNECTION_H

#include "esp_err.h"

esp_err_t mqtt_client_init(void);
esp_err_t mqtt_client_start(void);
esp_err_t mqtt_client_publish(const char *topic, const char *data, int len, int qos, int retain);
esp_err_t mqtt_client_subscribe(const char *topic, int qos);

#endif /* MQTT_CONNECTION_H */