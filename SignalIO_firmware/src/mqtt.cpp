#include <PubSubClient.h>
#include <Arduino.h>
// #include <WiFiClient.h>
#include "network.h"
// #include <SPI.h>
// #include <WiFi.h>
// #include "config_struct.h"
// #include "flash_mem.h"

// const char* mqttServer = "broker.shiftr.io";
// const char* mqttUser = "da4f1d0e";
// const char* mqttPassword = "72b8e7c8e7efea4d";

//const char* device_id = "Sensor_1";
// const char* mqttServer = "192.168.0.104";
// const char* mqttUser = "esp8266";
// const char* mqttPassword = "user1234";
const char* message;

// flash MEMORY;
// WiFiClient wifiClient;
PubSubClient clientMQTT(null);


// String message_mqtt;

void reconnect(const char* topic, const char* device_id, const char* mqttUser, const char* mqttPassword)
{
  while (!clientMQTT.connected())
  {
    Serial.println("Attempting MQTT connection to: ");
    Serial.println(topic);
    if (clientMQTT.connect(device_id, mqttUser, mqttPassword))
    { 
    Serial.println("connected\n");
    clientMQTT.subscribe(topic);
    }
    else
    {
      Serial.println("failed, rc=\n");
      Serial.println(clientMQTT.state());
      Serial.println(" try again in 5 seconds\n");
      delay(5000);
    }
  }
}


void callback(char *msg, byte *payload, unsigned int length){

    //message = callback_read();
    message = "test";
    String callback_message;

    for (unsigned int i = 0; i < length; i++) {
       callback_message += (char)payload[i];
    }
    //TODO
    if(callback_message == message){
        digitalWrite(UNIVERSAL_PIN_ONE, LOW);
    }
    else{
        digitalWrite(UNIVERSAL_PIN_ONE, HIGH);
    }
}


bool network::mqtt_connect(const char* device_id, const char* mqttUser, const char* mqttPassword, const char* mqttServer, String topic[], size_t len, String message){
    while (!clientMQTT.connected())
    {   
        clientMQTT.setServer(mqttServer, mqttPort); 
        if (clientMQTT.connect(device_id, mqttUser, mqttPassword))
        { 
            clientMQTT.setServer(mqttServer, mqttPort);
            for (size_t i = 0; i <= len-1; i++)
            {
                clientMQTT.subscribe(topic[i].c_str());
            }
            clientMQTT.setCallback(callback);
        }
        else
        {
        return false;
        }
    } 
    return true;
}


void network::mqtt_pub(String data, String topic, const char* device_id, const char* mqttUser, const char* mqttPassword){
    if (!clientMQTT.connected())
    {
        reconnect(topic.c_str(), device_id, mqttUser, mqttPassword);
    }
    clientMQTT.publish(topic.c_str(), data.c_str());
}


void network::mqtt_sub(String topic, const char* device_id, const char* mqttUser, const char* mqttPassword){
    if (!clientMQTT.connected())
    {
        reconnect(topic.c_str(), device_id, mqttUser, mqttPassword);
    }
    clientMQTT.loop();
}