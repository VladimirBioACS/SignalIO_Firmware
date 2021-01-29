#include <Arduino.h>
#include <PubSubClient.h>
#include "wifi_conn.h"
#include "json_lib.h"
#include "json_msg_packer.h"
#include "system_codes.h"
#include "file_system.h"

#ifndef NETWORK_H
#define NETWORK_H

class mqtt
{
private:
    const int port = 80;
    const int mqttPort = 1883;

public:
    const char* device_id;
    const char* mqttServer;
    const char* mqttUser;
    const char* mqttPassword;
    const char* topic;
    const char* callback_msg;
    int save_actuator_state_flag;

    uint8_t sensor_port;

    bool mqtt_connect();
    void mqtt_pub(String data, const char* topic);
    void mqtt_sub();
    void topic_sub();

};

#endif