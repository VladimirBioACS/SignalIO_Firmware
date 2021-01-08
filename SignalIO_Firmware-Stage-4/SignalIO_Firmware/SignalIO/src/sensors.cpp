#include "Arduino.h"
#include "sensors.h"
#include <DHT.h>
#include <DHT_U.h>
#include "DHT.h"

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
    pinMode(UNIVERSAL_PIN_ONE, INPUT);
    for(int i=0; i<=calibration_time; i++){
        delay(1000);
    }
    
}

int sensors::digital_sensor_read(){
    int state = digitalRead(UNIVERSAL_PIN_ONE);
    return state;
}

float sensors::analog_sensor_read(){
    float state = analogRead(UNIVERSAL_PIN_ONE);
    return state;
}