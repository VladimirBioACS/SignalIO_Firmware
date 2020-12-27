#include "mqtt.h"
#include "json_lib.h"

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
    //message = "test";

    StaticJsonDocument<200> msg_callback;
    StaticJsonDocument<200> document;

    String callback_message;
    String state_changer_on = "ON";
    String state_changer_off = "OFF";

    for (unsigned int i = 0; i < length; i++) {
       callback_message += (char)payload[i];
    }
    Serial.println(callback_message);

    DeserializationError error = deserializeJson(msg_callback,callback_message);
    
    const char* validation = msg_callback["type"];
    const char* message = msg_callback["actuator_state"];

    String actuator_state = "{\"actuator_state\":\"" + String(message)  + "\",\"type\":\"response\"}";
    
    //TODO
    if(String(validation) == "request"){
        if(String(message) == state_changer_on){
            digitalWrite(UNIVERSAL_PIN_ONE, LOW);
            Serial.println("actuator on");
            clientMQTT.publish("SignalIO/test", actuator_state.c_str());
        }
        if(String(message) == state_changer_off){
            digitalWrite(UNIVERSAL_PIN_ONE, HIGH);
            Serial.println("actuator off");
            clientMQTT.publish("SignalIO/test", actuator_state.c_str());
        }
    }
    
    if(error){
        Serial.println(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
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