#include <Arduino.h>
#include "json_lib.h"
#include "file_system.h"
#include "sensors.h"
#include "wifi_conn.h"
#include "mqtt.h"
#include "config_menu.h"
#include "json_msg_packer.h"
#include "system_codes.h"
#include "deep_sleep.h"
#include <Pangodream_18650_CL.h>

uint8_t sensor_select;

bool debug_state;
bool config_state;
bool dht_init;
bool sensors_init;
bool state;
bool deep_sleep_flag;
bool relay_pin = false;

String service_topic;
                 
const char* config_path = "/config.json";

unsigned int deep_sleep_timer;
int deep_sleep_mode;

FileSystem fileSystem;
sensors sensor; 
wifiConn WifiConn;
mqtt MQTT;
MessagePacker packer;
Pangodream_18650_CL bat;
Sleep slp;

StaticJsonDocument<1024> config;


void load_params(){
  MQTT.device_id = config["alias"];
  MQTT.mqttUser = config["mqtt_user"];
  MQTT.mqttPassword = config["mqtt_pass"];
  MQTT.mqttServer = config["mqtt_brocker"];
  MQTT.topic = config["mqtt_topic"];
  MQTT.callback_msg = config["mqtt_callback"];
  MQTT.sensor_port = atoi(config["module_port"]);

  sensor.module_pin = atoi(config["module_port"]);
}


void signal_led(int time){
  digitalWrite(SIGNAL_LED, HIGH);
  delay(time);
  digitalWrite(SIGNAL_LED, LOW);
  delay(time);
}


void deep_sleep_wakeup_reason(){
  const char* sensor_message = config["mqtt_callback"];
  
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 :  MQTT.mqtt_pub(packer.pack(sensor_message, "digital_sensor", STRING), config["mqtt_topic"]); break;
    //case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}

//TODO -- add Wi-Fi reset
void sys_reset(){
  int state = digitalRead(FACT_RESET_BTN);
  if(state == LOW){
     bool sys_reset_flag = fileSystem.config_reset(config_path);
     if(sys_reset_flag){
       Serial.println("Config erased");
       for (size_t i = 0; i < 3; i++)
       {
         signal_led(1000);
       }
       ESP.restart();
     }
  }
}

//TODO -- test/calibrate
void pwr_manager(){
  analogRead(CHARGE_MONITORING_ADC_PIN);
  int battery_charge_level = bat.getBatteryChargeLevel();
  MQTT.mqtt_pub(packer.pack(String(battery_charge_level).c_str(), "battery_charge_level", STRING), service_topic.c_str());
}


void dht_sensor(){
  if(dht_init){
  float temperature = sensor.read_temp();
  float humidity = sensor.read_hum();

  if(temperature == 0 || humidity == 0){
    MQTT.mqtt_pub(packer.message(DHT_SENSOR_NOT_FOUND, "error"), service_topic.c_str());
    signal_led(1000);
  }
  else{
  Serial.println(temperature); //Debug msg
  Serial.println(humidity); //Debug msg
      
  MQTT.mqtt_pub(packer.pack("23.00", "temperature", STRING), config["mqtt_topic"]);
  MQTT.mqtt_pub(packer.pack("40.00","humidity", STRING), config["mqtt_topic"]);
  MQTT.mqtt_sub();

  signal_led(1000);
  }
  }
  else
  {
    sensor.dht11_init();
    dht_init = true;
    Serial.println("dht11 setup done!\n"); //Debug msg
  }
}


void pir_motion_sensor(){
    const char* sensor_message = config["mqtt_callback"];
    if(sensors_init){
    int res = sensor.digital_sensor_read();
    if(res == HIGH){
      if(!state){
        Serial.println(sensor_message); //Debug msg
        MQTT.mqtt_pub(packer.pack(sensor_message, "PIR_sensor", STRING), config["mqtt_topic"]);
        MQTT.mqtt_sub();
        signal_led(100);
      }
      state = true;
    }
    else
    {
      state = false;
      delay(100);
    }
  }
  else
  {
    Serial.println("PIR sensor calibration");
    digitalWrite(SIGNAL_LED, HIGH);
    sensor.sensor_init();
    sensors_init = true;
    Serial.println("PIR sensor calibration done!\n"); //Debug msg
    digitalWrite(SIGNAL_LED, LOW);
  }
}


void digital_sensor(){
    const char* sensor_message = config["mqtt_callback"];
    //if(sensors_init){
    int res = sensor.digital_sensor_read();
    if(res == LOW){
        Serial.println(sensor_message);
        MQTT.mqtt_pub(packer.pack(sensor_message, "digital_sensor", STRING), config["mqtt_topic"]);
        MQTT.mqtt_sub();
        signal_led(1000);
    }
  //}
  // else
  // {
  //   Serial.println("Digital sensor calibration"); //Debug msg
  //   digitalWrite(SIGNAL_LED, HIGH);
  //   sensor.sensor_init();
  //   sensors_init = true;
  //   Serial.println("Digital sensor calibration done!"); //Debug msg
  //   digitalWrite(SIGNAL_LED, LOW);
  // }
}


void analog_sensor(){
    int analog_sensor_message = sensor.analog_sensor_read();
    MQTT.mqtt_pub(packer.pack(String(analog_sensor_message).c_str(), "analog sensor", STRING), config["mqtt_topic"]);
    MQTT.mqtt_sub();
    signal_led(1000);
}


void relay(){
 if(relay_pin){
    MQTT.mqtt_sub();
  }
  else
  {
    Serial.println("Actuator module selected");
    MQTT.topic_sub(); // may not need
    sensor.relay_init();
    relay_pin = true;
    Serial.println("Actuator pin configuration done"); //Debug msg
  }
}


void setup()
{
  pinMode(SIGNAL_LED, OUTPUT);
  pinMode(FACT_RESET_BTN, INPUT_PULLUP);

  Serial.begin(9600);

  for (size_t i = 0; i < 5; i++)
  {
    signal_led(250);
  }

  if(!SPIFFS.begin()){
    Serial.println("Failed to init file system");
    signal_led(5000);
    ESP.restart();
  }

  Serial.println("File system inited");

  bool file_state = fileSystem.read_file("/config.json");

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
          config_menu();
        }

        load_params();
        deep_sleep_flag = atoi(config["deepsleep_flag"]);
        bool mqtt_state = MQTT.mqtt_connect();
        if(mqtt_state){
          Serial.println("MQTT connected!");
        }
        else
        {
          Serial.println("MQTT error. Not connected");
        }    
    }    
    else
    {
      Serial.println("Wifi not connected");
      WifiConn.wifi_manager();
      delay(5000);
      ESP.restart();
    }

    sensor_select = atoi(config["module"]);
    Serial.printf("Module: %x\n", sensor_select);
  }
  // TODO
  if(!file_state){
    Serial.println("File not found");
    config_menu();
  }

  digitalWrite(SIGNAL_LED, LOW);
  MQTT.topic_sub();
  const char* alias_dev = config["alias"];
  service_topic = "error/" + String(alias_dev);
  
  if(deep_sleep_flag){
    Serial.println("Enable deep sleep mode");
    deep_sleep_mode = atoi(config["deepsleep_type"]);
    Serial.println(deep_sleep_mode);
    if(deep_sleep_mode == TMR_MODE){
        Serial.println("timer mode");
        deep_sleep_timer = atoi(config["deepseep_tmr"]);
        sys_reset();

        switch (sensor_select)
        {
        case DIGITAL_SENSOR:
          sensor.sensor_init();
          delay(3000);
          digital_sensor();
          slp.tmr_sleep(deep_sleep_timer);
          break;

        case ANALOG_SENSOR:
          delay(3000);
          analog_sensor();
          slp.tmr_sleep(deep_sleep_timer);
          break;
        
        default:
          Serial.println("module not supported by deep sleep mode"); // debug
          MQTT.mqtt_pub(packer.message(MODULE_NOT_SUPPORTED_BY_DEEPSLEEP, "error"), service_topic.c_str());
          signal_led(1000);
          break;
        }
    }

    if(deep_sleep_mode == PIN_TRIGGER_MODE){
        deep_sleep_wakeup_reason();
        Serial.println("trigger mode");
        Serial.println("!!!WARNING!!!\nThis function supports only digital sensors on RTC pins"); // delete this
        pinMode(14, OUTPUT_OPEN_DRAIN);
        digitalWrite(14, LOW);
        slp.pin_trigger_sleep();
    }
  }
  Serial.println("Done");
}


void loop() 
{
  sys_reset();

  switch (sensor_select)
  {
  case DHT11_SENSOR:
    dht_sensor();
    break;
  
  case DIGITAL_SENSOR:
    if(!sensors_init){
        Serial.println("Digital sensor calibration"); //Debug msg
        digitalWrite(SIGNAL_LED, HIGH);
        sensor.sensor_init();
        sensors_init = true;
        Serial.println("Digital sensor calibration done!"); //Debug msg
        digitalWrite(SIGNAL_LED, LOW);
      }
    digital_sensor();
    break;

  case ANALOG_SENSOR:
    analog_sensor();
    break;

  case PIR_SENSOR:
    pir_motion_sensor();
    break;

  case RELAY:
    relay();
    break;
  
  default:
    Serial.println("No module selected"); // debug
    MQTT.mqtt_pub(packer.message(MODULE_NOT_FOUND, "error"), service_topic.c_str());
    signal_led(1000);
    break;
  }
}