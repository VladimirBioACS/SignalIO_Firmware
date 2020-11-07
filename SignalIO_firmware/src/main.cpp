#include <Arduino.h>
#include "sensors.h"
#include "system_signal.h"

int sensor_select;
bool debug_state;
bool dht_init;
bool sensors_init;
bool state;

String topic[] = {"test/test", "test2/test2"};

sensors sensor;

void sys_error(){
  for(int i = 0; i<=5; i++){
    signal_led(2);
  }
}

void sys_reset(){
  //TODO
}

void pwr_manager(){
  //TODO
}

void dht_sensor(){
    if(dht_init){
    float temperature = sensor.read_temp();
    float humidity = sensor.read_hum();
    if(temperature == 0 || humidity == 0){
      if(debug_state){
        Serial.println("DHT11 Module error!!!\n");
        signal_led(1000);
      }
    }else{
      if(debug_state){
        Serial.println(temperature); //Debug msg
        Serial.println(humidity); //Debug msg
        signal_led(1000);
      }
      String dht_json_msg = "{\"temperature\":" + String(temperature) + ", \"humidity\":" + String(humidity) + "}";
      Serial.println(dht_json_msg);
    }
    delay(1000);
  }
  else
  {
    sensor.dht11_init();
    dht_init = true;
    if(debug_state){
      Serial.println("dht11 setup done!\n"); //Debug msg
    }
  }

}

void digital_sensor(){
    if(sensors_init){
    int res = sensor.digital_sensor_read();
    if(res == HIGH){
      if(!state){
        if(debug_state){
          Serial.println("Digital sensor activated"); //Debug msg
          Serial.println("\n");
          signal_led(1000);
        }
      }
      state = true;
    }
    else
    {
      state = false;
      delay(1000);
    }
  }
  else
  {
    digitalWrite(13, LOW);
    sensor.sensor_init();
    sensors_init = true;
    if(debug_state){
      Serial.println("Digital sensor calibration done!\n"); //Debug msg
    }
    digitalWrite(13, HIGH);
  }
}

void analog_sensor(){
 if(sensors_init){
    float res = sensor.analog_sensor_read();
    Serial.println(res); //Debug msg
    signal_led(1000);
  }
  else
  {
    digitalWrite(13, LOW);
    sensor.sensor_init();
    sensors_init = true;
    if(debug_state){
      Serial.println("Analog sensor calibration done!\n"); //Debug msg
    }
    digitalWrite(13, HIGH);
  }
}

void relay(){
 // TODO. Needed MQTT
}

void setup()
{
  pinMode(13, OUTPUT);

  Serial.begin(9600);
  Serial.println("System init...");

  sensor_select = 0xC7;

  if(sensor_select == DHT11){
    Serial.println("DHT11 module selected\n");
  }else if(sensor_select == DIGITAL_SENSOR){
    Serial.println("Digital sensor selected\n");
  }else if (sensor_select == ANALOG_SENSOR){
    Serial.println("Analog sensor selected\n");
  }else if(sensor_select == RELAY){
    Serial.println("Relay module selected\n");
  }
}

void loop() 
{

  switch (sensor_select)
  {
  case DHT11:
    dht_sensor();
    break;
  
  case DIGITAL_SENSOR:
    digital_sensor();
    break;

  case ANALOG_SENSOR:
    analog_sensor();
    break;

  case RELAY:
    relay();
    break;
  
  default:
    break;
  }
}
