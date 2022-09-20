#include "MQTTClient.h"
// Broker: broker.emqx.io
// TCP Port: 1883
// Websocket Port: 8083
// TCP/TLS Port: 8883
// Websocket/TLS Port: 8084

#define ADDRESS     "ws://broker.emqx.io:8083"
#define CLIENTID_PUB    "Your_Client_ID"
// #define SUB_TOPIC   "test/topic1"
#define PUB_TOPIC   "test/MPU"
#define PUB_TOPIC_2   "test/MPU_2"
#define QOS         1 

void publish(MQTTClient client, char* topic, char* payload) {
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    pubmsg.payload = payload;
    pubmsg.payloadlen = strlen(pubmsg.payload);
    pubmsg.qos = 1;
    pubmsg.retained = 0;
    MQTTClient_deliveryToken token;
    MQTTClient_publishMessage(client, topic, &pubmsg, &token);
    MQTTClient_waitForCompletion(client, token, 1000L);
    printf("Message '%s' with delivery token %d delivered\n", payload, token);
}

