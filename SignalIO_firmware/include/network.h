#include <Arduino.h>

#ifndef NETWORK_H
#define NETWORK_H


class network
{
private:
    const int port = 80;
    const int mqttPort = 1883;

public:
    network(/* args */);
    ~network();
    bool mqtt_connect(const char* device_id, const char* mqttUser, const char* mqttPassword, const char* mqttServer, String topic[], size_t len, String message);
    void mqtt_pub(String data, String topic, const char* device_id, const char* mqttUser, const char* mqttPassword);
    void mqtt_sub(String topic, const char* device_id, const char* mqttUser, const char* mqttPassword);

};

network::network(/* args */)
{
}

network::~network()
{
}



#endif