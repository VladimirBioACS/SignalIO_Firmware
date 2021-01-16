#include <Arduino.h>
#include <PubSubClient.h>
#include "wifi_conn.h"
#include "json_lib.h"
#include "json_msg_packer.h"
#include "error_codes.h"

#ifndef NETWORK_H
#define NETWORK_H

class mqtt
{
private:
    // const char* device_id = "SignalIO_Sensor_1";
    // const char* mqttServer = "broker.shiftr.io"; //"signalio.cloud.shiftr.io";
    // const char* mqttUser =  "da4f1d0e"; //"signalio"; 
    // const char* mqttPassword =  "72b8e7c8e7efea4d"; //"I2AkSPlg8QF6kbrY";

    const int port = 80;
    const int mqttPort = 1883;

public:
    const char* device_id;
    const char* mqttServer;
    const char* mqttUser;
    const char* mqttPassword;
    const char* topic;
    const char* callback_msg;
    const char* service_topic = "Sensor/Service";

    uint8_t sensor_port;

    bool mqtt_connect();
    void mqtt_pub(String data);
    void mqtt_sub();
    void topic_sub();

};

// mqtt::mqtt(/* args */)
// {
// }

// mqtt::~mqtt()
// {
// }

#endif