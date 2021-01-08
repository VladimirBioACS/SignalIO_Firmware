#include <Arduino.h>
#include "json_lib.h"
#include "file_system.h"
#include "sensors.h"
#include "wifi_conn.h"
#include "mqtt.h"
#include "config_menu.h"

int sensor_select;
uint8_t fact = 23;

bool debug_state;
bool config_state;
bool dht_init;
bool sensors_init;
bool state;
bool relay_pin = false;
                 
const char* mqtt_topic = "SignalIO/test";
const char* config_path = "/config.json";

FileSystem fileSystem;
sensors sensor; 
wifiConn WifiConn;
mqtt MQTT;

StaticJsonDocument<1024> config;

void signal_led(int time){
  digitalWrite(SIGNAL_LED, HIGH);
  delay(time);
  digitalWrite(SIGNAL_LED, LOW);
  delay(time);
}

//TODO
void sys_reset(){
  int state = digitalRead(fact);
  if(state == LOW){
     bool sys_reset_flag = fileSystem.config_reset(config_path);
     if(sys_reset_flag){
       Serial.println("Config erased");
       ESP.restart();
     }
  }
}

//TODO
void pwr_manager(){
  //TODO
}

void error(){
  char buf[20];
  DynamicJsonDocument err(150);
  JsonObject message = err.to<JsonObject>();
  message["error"] = "1";
  serializeJson(message, buf);
  MQTT.mqtt_pub(String(buf));
}

void dht_sensor(){
    DynamicJsonDocument sensor_data(150);
    DynamicJsonDocument sensor_data_container(150);
    JsonObject msg = sensor_data.to<JsonObject>();

    char buff[255];

    // if(dht_init){
    // float temperature = 22.0;//sensor.read_temp();
    // float humidity = 22.0;//sensor.read_hum();

    // if(temperature == 0 || humidity == 0){
    //   if(debug_state){
    //     Serial.println("DHT11 Module error!!!\n");
    //     error();
    //     signal_led(1000);
    //   }
    // }else{
      // if(debug_state){
      //   Serial.println(temperature); //Debug msg
      //   Serial.println(humidity); //Debug msg
      //   signal_led(1000);
      // }

      msg["name"] = "temperature";
      msg["value"] = "23.00";
      msg["type"] = "string";

      JsonArray data = sensor_data_container.createNestedArray("props");
      data.add(msg);
      
      serializeJsonPretty(sensor_data_container, buff);

      //String dht_json_msg = "{\"temperature\":" + String(temperature) + ", \"humidity\":" + String(humidity) + "}";
      MQTT.mqtt_sub();
      error();
      //MQTT.mqtt_pub(String(buff));
    //}
    delay(1000);
  }
  // else
  // {
  //   sensor.dht11_init();
  //   dht_init = true;
  //   if(debug_state){
  //     Serial.println("dht11 setup done!\n"); //Debug msg
  //   }
  // }

//}

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
 if(relay_pin){
    MQTT.mqtt_sub();
  }
  else
  {
    pinMode(UNIVERSAL_PIN_ONE, OUTPUT);
    digitalWrite(UNIVERSAL_PIN_ONE, HIGH);
    relay_pin = true;
    if(debug_state){
      Serial.println("Actuator pin configuration done"); //Debug msg
    }
  }
}

void setup()
{
  delay(5000);
  pinMode(SIGNAL_LED, OUTPUT);
  pinMode(fact, INPUT_PULLUP);
  Serial.begin(9600);

  if(!SPIFFS.begin()){
    Serial.println("Failed to init file system");
    signal_led(5000);
    ESP.restart();
  }

  Serial.println("File system inited");

  bool file_state = fileSystem.read_file("/config.json");
  // Debug region
  Serial.println("\n");
  bool file_state_wifi = fileSystem.read_file("/wifi_creds.json");
  Serial.println("\n");
  bool file_state_creds = fileSystem.read_file("/auth_creds.json");
  Serial.println("\n");
  // Debug region end
 
  if(file_state){
    config = fileSystem.get_config(config_path);
    config_state = config["config_flag"];

    Serial.println("System init...");
    bool state = WifiConn.wifi_connect();
    if(state){
        Serial.println("Wi-Fi connected");
        WifiConn.printCurrentNet();
        WifiConn.printWifiData();
  
        if(config_state){
          Serial.println("Config server started\nUse your local IP to connect");
          //load_auth_creds();
          config_menu();
        }

        bool mqtt_state = MQTT.mqtt_connect();
        if(mqtt_state){
          Serial.println("MQTT connected!");
        }
        else
        {
          Serial.println("MQTT error. Not connected");
          sys_reset();
        }    
    }    
    else
    {
      //TODO
      Serial.println("Wifi not connected");
      WifiConn.wifi_manager();
      delay(5000);
      ESP.restart();
    }

    sensor_select = atoi(config["module"]);
    Serial.printf("Sensor: %x\n", sensor_select);
    for (size_t i = 0; i < 5; i++)
    {
      signal_led(500);
    }
  }
  // TODO
  if(!file_state){
    Serial.println("File not found");
    config_menu();
  }

  //Debug 
  serializeJson(config, Serial);

  const char* mqtt_topic = config["mqtt_topic"];
  const char* alias = config["alias"];
  int module = atoi(config["module"]);
  
  Serial.println("\n\n");
  Serial.println(mqtt_topic);
  Serial.println(alias);
  Serial.println(module);
  Serial.println("\n\n");
  
  Serial.println("Config deserialized");
  //Debug end
}


void loop() 
{
  sys_reset();

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
    Serial.println("No module selected");
    signal_led(1000);
    break;
  }
}