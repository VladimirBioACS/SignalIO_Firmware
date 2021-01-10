#include "sensors.h"

DHT dht(UNIVERSAL_PIN_ONE, DHTTYPE);

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
    for(int i=0; i<=calibration_time; i++){
        delay(1000);
    }
}

int sensors::digital_sensor_read(){
    int state = digitalRead(module_pin);
    return state;
}

int sensors::analog_sensor_read(){
    int state = analogRead(module_pin);
    return state;
}