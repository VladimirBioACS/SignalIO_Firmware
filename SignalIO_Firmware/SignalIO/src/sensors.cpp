#include "sensors.h"

sensors sensor_pin;
FileSystem actuator_file_system;
DHT dht(sensor_pin.module_pin, DHTTYPE);


void sensors::dht11_init(void){
    dht.begin();
}


float sensors::read_hum(void){
    hum = dht.readHumidity();
    if(isnan(hum)){
        return 0;
    }else
    {
        return hum;
    }
}


float sensors::read_temp(void){
    temp = dht.readTemperature();
    if(isnan(temp)){
        return 0;
    }else
    {
        return temp;
    }
}


void sensors::sensor_init(){
    pinMode(module_pin, INPUT);
    pinMode(14, OUTPUT_OPEN_DRAIN);
    digitalWrite(14, LOW);
}


int sensors::digital_sensor_read(){
    int state = digitalRead(module_pin);
    return state;
}


int sensors::analog_sensor_read(){
    int state = analogRead(module_pin);
    return state;
}


void sensors::relay_init(){
    StaticJsonDocument<20> actuator_status;
    actuator_status = actuator_file_system.get_config("/actuator_state.json");
    int actuator_state = atoi(actuator_status["state"]);
    pinMode(module_pin, OUTPUT_OPEN_DRAIN);
    switch (actuator_state)
    {
    case 1:
        digitalWrite(module_pin, LOW);
        break;
    
    case 0:
        digitalWrite(module_pin, HIGH);
        break;
    
    default:
        break;
    }
}