#include "mqtt.h"

const char* message;

const char* mqtt_callback_message;
const char* mqtt_topic;
bool save_actuator_state;

uint8_t module_pin;

WiFiClient wifiClient;
PubSubClient clientMQTT(wifiClient);
mqtt actuator_mqtt;
MessagePacker relay_msg_packer;
FileSystem actuator_write_state;


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


void actuator_callback(char *msg, byte *payload, unsigned int length){

    StaticJsonDocument<200> msg_callback;
    //StaticJsonDocument<200> document;
    
    DynamicJsonDocument actuator_cur_state(20);
    char actuator_cur_state_buff[20];

    String received_message;
    String state_changer_on = mqtt_callback_message;

    const char* validation;
    const char* message;

    for (unsigned int i = 0; i < length; i++) {
       received_message += (char)payload[i];
    }
    Serial.println(received_message); // debug

    DeserializationError error = deserializeJson(msg_callback,received_message);

    validation = msg_callback["props"][0]["type"];
    message = msg_callback["props"][0]["value"];

    if(String(validation) == "request"){
        if(String(message) == state_changer_on){
            digitalWrite(module_pin, LOW);
            clientMQTT.publish(mqtt_topic, relay_msg_packer.pack(message, "actuator", RESPONSE).c_str());
            
            if(save_actuator_state){
                actuator_cur_state["state"] = "1";
                serializeJson(actuator_cur_state, actuator_cur_state_buff);

                bool save_flag = actuator_write_state.write_file("/actuator_state.json", actuator_cur_state_buff);
                if(!save_flag){
                    Serial.println("State write failed!");
                }
            }
        }
        if(String(message) != state_changer_on){
            digitalWrite(module_pin, HIGH);
            clientMQTT.publish(mqtt_topic, relay_msg_packer.pack(message, "actuator", RESPONSE).c_str());
            
            if(save_actuator_state){
                actuator_cur_state["state"] = "0";
                serializeJson(actuator_cur_state, actuator_cur_state_buff);

                bool save_flag = actuator_write_state.write_file("/actuator_state.json", actuator_cur_state_buff);
                if(!save_flag){
                    Serial.println("State write failed!");
                }
            }
        }
    }
    
    if(error){
        Serial.println(F("deserializeJson() failed: ")); // debug
        Serial.println(error.c_str()); // debug
        clientMQTT.publish(mqtt_topic, relay_msg_packer.message(ACTUATOR_REQUEST_NOT_RECOGNIZED, "error").c_str());
    }
}


bool mqtt::mqtt_connect(){
    module_pin = sensor_port;
    mqtt_callback_message = callback_msg;
    mqtt_topic = topic;
    save_actuator_state = save_actuator_state_flag;

    while (!clientMQTT.connected())
    {   
        clientMQTT.setServer(mqttServer, mqtt_port); 
        if (clientMQTT.connect(device_id, mqttUser, mqttPassword))
        { 
            clientMQTT.setServer(mqttServer, mqtt_port);
            clientMQTT.setCallback(actuator_callback);
        }
        else
        {
        return false;
        }
    } 
    return true;
}


void mqtt::mqtt_pub(String data, const char* topic){
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