#include <Arduino.h>
#include "sensors.h"
#include "wifi_conn.h"
#include "mqtt.h"

int sensor_select;
bool debug_state;
bool dht_init;
bool sensors_init;
bool state;
                 
const char* mqtt_topic = "SignalIO/test";

sensors sensor; 
wifiConn WifiConn;
mqtt MQTT;


void signal_led(int time){
  digitalWrite(SIGNAL_LED, HIGH);
  delay(time);
  digitalWrite(SIGNAL_LED, LOW);
  delay(time);
}

//TODO
void sys_reset(){
  for (size_t i = 0; i < 5; i++)
  {
    signal_led(100);
  }
  ESP.restart();
  
}

//TODO
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
        Serial.println("Digital sensor activated"); //Debug msg
        Serial.println("\n");
        MQTT.mqtt_pub("HIGH");
        signal_led(1000);
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
    digitalWrite(SIGNAL_LED, HIGH);
    sensor.sensor_init();
    sensors_init = true;
    if(debug_state){
      Serial.println("Digital sensor calibration done!\n"); //Debug msg
    }
    digitalWrite(SIGNAL_LED, LOW);
  }
}

void analog_sensor(){
 if(sensors_init){
    float res = sensor.analog_sensor_read();
    Serial.println(res); //Debug msg
    MQTT.mqtt_pub(String(res));
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
 // TODO. MQTT needed
}


void setup()
{
  pinMode(UNIVERSAL_PIN_ONE, INPUT_PULLUP);
  pinMode(SIGNAL_LED, OUTPUT);
  Serial.begin(9600);
  sensor_select = 0xC6;

  for (size_t i = 0; i < 5; i++)
  {
    signal_led(500);
  }

  Serial.println("System init...");
  bool state = WifiConn.wifi_connect();
  if(state){
      Serial.println("Wi-Fi connected");
      WifiConn.printCurrentNet();
      WifiConn.printWifiData();
      bool mqtt_state = MQTT.mqtt_connect();
      if(mqtt_state){
        Serial.println("MQTT connected!");
        Serial.printf("Sensor: %x", sensor_select);
      }
      else
      {
        Serial.println("MQTT error. Not connected");
        sys_reset();
      }    
  }    
  else
  {
    Serial.println("Wifi not connected");
    sys_reset();
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