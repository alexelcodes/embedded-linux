#include <string.h>
#include "mqtt.h"

#define CLIENTID "rpi-client"
#define QOS 1
#define TIMEOUT 10000L

MQTTClient mqtt_init(const char *address)
{
    MQTTClient client;
    MQTTClient_connectOptions opts = MQTTClient_connectOptions_initializer;

    MQTTClient_create(&client, address, CLIENTID,
                      MQTTCLIENT_PERSISTENCE_NONE, NULL);

    opts.keepAliveInterval = 20;
    opts.cleansession = 1;

    MQTTClient_connect(client, &opts);

    return client;
}

void mqtt_publish(MQTTClient client, const char *topic, const char *payload)
{
    MQTTClient_message msg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    msg.payload = (void *)payload;
    msg.payloadlen = strlen(payload);
    msg.qos = QOS;
    msg.retained = 0;

    MQTTClient_publishMessage(client, topic, &msg, &token);
    MQTTClient_waitForCompletion(client, token, TIMEOUT);
}