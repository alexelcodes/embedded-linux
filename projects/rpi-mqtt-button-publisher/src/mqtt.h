#ifndef MQTT_H
#define MQTT_H

#include "MQTTClient.h"

MQTTClient mqtt_init(const char *address);
void mqtt_publish(MQTTClient client, const char *topic, const char *payload);

#endif