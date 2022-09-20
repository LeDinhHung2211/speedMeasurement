#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "lib/mqtt_pub/mqtt_pub.h"
#include "lib/mqtt_sub/mqtt_sub.h"
#include "lib/MPU6050/MPU6050.h"


// Broker: broker.emqx.io
// TCP Port: 1883
// Websocket Port: 8083
// TCP/TLS Port: 8883
// Websocket/TLS Port: 8084

extern int mpu_address;
extern int rawData[5];
/* extern int rawData[3]; */
MPU6050 ACC;
MPU6050 GYRO;
char data[100];
char *fullScale_acc;
char *fullScale_gyro;

uint16_t accSens = 4096;
long accSensitivity(int mode)
{
	/* return 40; */
	switch (mode)
	{
		case 0: fullScale_acc = "2g";
				return 16384;
				break;
		case 1: fullScale_acc = "4g";
				return 8192;
				break;
		case 2: fullScale_acc = "8g";
				return 4096;
				break;
		case 3: fullScale_acc = "16g";
				return 2048;
				break;
		default: break;
	}
}

float gyroSens = 16.4;
float gyroSensitivity(uint8_t mode)
{
	switch (mode)
	{
		case 0: fullScale_gyro = "250 deg/s";
				return 131;
				break;
		case 1: fullScale_gyro = "500 deg/s";
				return 65.5;
				break;
		case 2: fullScale_gyro = "1000 deg/s";
				return 32.8;
				break;
		case 3: fullScale_gyro = "2000 deg/s";
				return 16.4;
				break;
	}
}



void mpu_interrupt()
{	
	ACC = read_sensor(ACC_X,accSens);
    sprintf(data,"{\"Ax\":%.2f,\"Ay\":%.2f,\"Az\":%.2f}",ACC.x,ACC.y,ACC.z);
	//GYRO = read_sensor(GYRO_X,65.5);
	//pitch = atan2(ACC.x, sqrt(pow(ACC.y,2)+pow(ACC.z,2)))*180/M_PI;//goc theo x
	//roll = atan2(ACC.y,sqrt(pow(ACC.x,2)+pow(ACC.z,2)))*180/M_PI;// goc theo y
	// yaw	= atan2(sqrt(pow(ACC.x,2)+pow(ACC.y,2)),ACC.z)*180/M_PI;// goc theo y
	wiringPiI2CReadReg8(mpu_address,58);
}

int on_message(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    /* float rawData[3]; */
	char* payload = message->payload;
    printf("Received message from topic %s: %s\n",topicName, payload);
	splitConvertData(payload);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
	/* check_topic(topicName); */
	/* flag_on_msg =1; */
	/* for (int i=0;i<size;i++)
	{
		printf("%.2f\n",rawData[i]);
	} */
	gyroSens = gyroSensitivity(rawData[2]);
	accSens = accSensitivity(rawData[4]);
	printf("Sample Rate Division: %d\n",rawData[0]);
	printf("DLPF: %d\n",rawData[1]);
	printf("Gyro Scale: %.2f(%s)\n",gyroSens,fullScale_gyro);
	printf("Acc Scale: %ld(%s)\n",accSens,fullScale_acc);
	
	
	initMpu6050(rawData[0],rawData[1],rawData[2],rawData[4]);
    return 1;
}


int main(int argc, char* argv[]) {
    wiringPiSetup();
	// wiringPiSPISetup(channel,speed);
	pinMode(INT, INPUT);
	wiringPiISR(INT,INT_EDGE_BOTH,&mpu_interrupt);
	mpu_address = wiringPiI2CSetup(0x68);
	// initMax7219();
	/* initMpu6050(); */
	uint8_t check;
	check = wiringPiI2CReadReg8(mpu_address,117);// TEST
	wiringPiI2CWriteReg8(mpu_address,0x38,0x01);//enable interupt for mpu
	if(check == 0x68){
		printf("MPU is ready\n");
	}
	else{
		printf("Check mpu again!\n");
	}
    MQTTClient client;
    MQTTClient_create(&client, ADDRESS, CLIENTID_PUB, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    

    MQTTClient_setCallbacks(client, NULL, NULL, on_message, NULL);
	/* MQTTClient_setCallbacks(client, NULL, NULL, on_message_2, NULL); */
    int rc;
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }
   
    //listen for operation
    MQTTClient_subscribe(client, SUB_TOPIC, 0);
	/* MQTTClient_subscribe(client, SUB_TOPIC_2, 0); */
    while(1) {

        publish(client, PUB_TOPIC, data);
        delay(1000);
    }
    MQTTClient_disconnect(client, 1000);
    MQTTClient_destroy(&client);
    return rc;
}