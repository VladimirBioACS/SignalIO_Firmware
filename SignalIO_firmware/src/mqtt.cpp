#include "mqtt.h"

// const char* device_id = "Sensor_1";
// const char* mqttServer = "broker.shiftr.io";
// const char* mqttUser = "da4f1d0e";
// const char* mqttPassword = "72b8e7c8e7efea4d";

// const char* mqttServer = "192.168.0.104";
// const char* mqttUser = "esp8266";
// const char* mqttPassword = "user1234";
const char* message;

// flash MEMORY;
WiFiClient wifiClient;
PubSubClient clientMQTT(wifiClient);


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


bool mqtt::mqtt_connect(){
    while (!clientMQTT.connected())
    {   
        clientMQTT.setServer(mqttServer, mqttPort); 
        if (clientMQTT.connect(device_id, mqttUser, mqttPassword))
        { 
            clientMQTT.setServer(mqttServer, mqttPort);
            // for (size_t i = 0; i <= len-1; i++)
            // {
            //     clientMQTT.subscribe(topic[i]);
            // }
            clientMQTT.subscribe(topic);
            clientMQTT.setCallback(callback);
        }
        else
        {
        return false;
        }
    } 
    return true;
}


void mqtt::mqtt_pub(String data){
    if (!clientMQTT.connected())
    {
        reconnect(topic, device_id, mqttUser, mqttPassword);
    }
    clientMQTT.publish(topic, data.c_str());
}


void mqtt::mqtt_sub(){
    if (!clientMQTT.connected())
    {
        reconnect(topic, device_id, mqttUser, mqttPassword);
    }
    clientMQTT.loop();
}