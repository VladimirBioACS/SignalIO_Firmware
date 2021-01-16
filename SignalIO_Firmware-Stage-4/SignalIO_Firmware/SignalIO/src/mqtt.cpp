#include "mqtt.h"

const char* message;

const char* mqtt_callback_message;
const char* mqtt_topic;
uint8_t module_pin;

WiFiClient wifiClient;
PubSubClient clientMQTT(wifiClient);
mqtt actuator_mqtt;
MessagePacker relay_msg_packer;

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

    StaticJsonDocument<200> msg_callback;
    StaticJsonDocument<200> document;

    String received_message;
    String state_changer_on = mqtt_callback_message;

    const char* validation;
    const char* message;

    for (unsigned int i = 0; i < length; i++) {
       received_message += (char)payload[i];
    }
    Serial.println(received_message); // debug

    DeserializationError error = deserializeJson(msg_callback,received_message);

    validation = msg_callback["type"];
    message = msg_callback["actuator_state"];

    //TODO
    if(String(validation) == "request"){
        if(String(message) == state_changer_on){
            digitalWrite(module_pin, LOW);
            Serial.println("actuator on"); // debug
            clientMQTT.publish(mqtt_topic, relay_msg_packer.pack(message, "actuator", RESPONSE).c_str());
        }
        if(String(message) != state_changer_on){
            digitalWrite(module_pin, HIGH);
            Serial.println("actuator off"); // debug
            clientMQTT.publish(mqtt_topic, relay_msg_packer.pack(message, "actuator", RESPONSE).c_str());
        }
    }
    
    if(error){
        Serial.println(F("deserializeJson() failed: ")); // debug
        Serial.println(error.c_str()); // debug
        clientMQTT.publish(mqtt_topic, relay_msg_packer.error(ACTUATOR_REQUEST_NOT_RECOGNIZED).c_str());
    }
}

bool mqtt::mqtt_connect(){
    module_pin = sensor_port;
    mqtt_callback_message = callback_msg;
    mqtt_topic = topic;

    while (!clientMQTT.connected())
    {   
        clientMQTT.setServer(mqttServer, mqttPort); 
        if (clientMQTT.connect(device_id, mqttUser, mqttPassword))
        { 
            clientMQTT.setServer(mqttServer, mqttPort);
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

void mqtt::topic_sub(){
    clientMQTT.subscribe(topic);
}

void mqtt::mqtt_sub(){
    if (!clientMQTT.connected())
    {
        reconnect(topic, device_id, mqttUser, mqttPassword);
    }
    clientMQTT.loop();
}