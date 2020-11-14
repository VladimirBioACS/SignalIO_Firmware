#include <Arduino.h>
#include <PubSubClient.h>
#include "wifi_conn.h"
// #include <WiFiClient.h>

#ifndef NETWORK_H
#define NETWORK_H

class mqtt
{
private:
    const char* device_id = "Sensor_1";
    const char* mqttServer = "broker.shiftr.io";
    const char* mqttUser = "da4f1d0e";
    const char* mqttPassword = "72b8e7c8e7efea4d";

    const int port = 80;
    const int mqttPort = 1883;
    const char* topic = "SignalIO/test";

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