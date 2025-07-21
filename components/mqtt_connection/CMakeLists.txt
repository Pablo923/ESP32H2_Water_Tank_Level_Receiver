idf_component_register(SRCS "mqtt_connection.c"
                       INCLUDE_DIRS "include"
                       REQUIRES esp_wifi esp_event mqtt)