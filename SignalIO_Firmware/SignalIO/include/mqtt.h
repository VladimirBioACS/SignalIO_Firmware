#include <Arduino.h>
#include <PubSubClient.h>
#include "wifi_conn.h"
// #include <WiFiClient.h>

#ifndef NETWORK_H
#define NETWORK_H

class mqtt
{
private:
    const char* device_id = "SignalIO_Sensor_1";
    const char* mqttServer = "broker.shiftr.io"; //"signalio.cloud.shiftr.io";
    const char* mqttUser =  "da4f1d0e"; //"signalio"; 
    const char* mqttPassword =  "72b8e7c8e7efea4d"; //"I2AkSPlg8QF6kbrY";

    const int port = 80;
    const int mqttPort = 1883;
    const char* topic = "SignalIO/test";
    const char* api_key = "";

public:
    // mqtt(const char* topic){
    //     topic = topic;
    // };
    // ~mqtt();
    bool mqtt_connect();
    void mqtt_pub(String data);
    void mqtt_sub();

};

// mqtt::mqtt(/* args */)
// {
// }

// mqtt::~mqtt()
// {
// }

#endif