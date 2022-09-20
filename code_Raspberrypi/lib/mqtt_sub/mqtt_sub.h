#ifndef MQTT_SUB_H
#define MQTT_SUB_H
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
// #include <mysql.h>
#include "MQTTClient.h"

#define ADDRESS     "ws://broker.emqx.io:8083"
#define CLIENTID_SUB    "subcsribe_demo"
#define SUB_TOPIC   "test/configMPU"
#define SUB_TOPIC_2   "test/configMPU_2"
/* #define PUB_TOPIC   "test/topic2" */
#define QOS         1
#define size 5
bool flag_on_msg =0;
/* void splitConvertData(char *string); */
int rawData[5];

void splitConvertData(char *string)
{
	/* char string[50] = "2340-4235-6112230"; */
  // Lấy token đầu tiên

    char * token = strtok(string, "/");
  int raw[5];
  int index =0;
  // Lấy ra toàn bộ token
  while( token != NULL ) {
     raw[index] = atoi(token);
     token = strtok(NULL, "/");
     index++;
  }
  for (int i=0;i<size;i++)
  {
    /* printf("%.2f\n",raw[i]);  */
	rawData[i] = raw[i];
    /* printf("%.2f\n",rawData[i]); */
  }
  
  /* data = &raw; */
}

void check_topic(char *topic)
{
	int check_topic1 = strcmp (topic, SUB_TOPIC);
	int check_topic2 = strcmp (topic, SUB_TOPIC_2);
	if (!check_topic1) printf("topic1\n");
	if (!check_topic2) printf("topic2\n");
}
#endif