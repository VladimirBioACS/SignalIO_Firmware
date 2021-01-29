#include "json_msg_packer.h"

String MessagePacker::pack(const char* sensor_data, const char* module_name, const char* type){
    DynamicJsonDocument msg_container(255);
    DynamicJsonDocument sensor_data_container(255);

    JsonObject msg = sensor_data_container.to<JsonObject>();

    msg["name"] = module_name;
    msg["value"] = sensor_data;
    msg["type"] = type;

    JsonArray data = msg_container.createNestedArray("props");
    char msg_buff[255];
    data.add(msg);
    serializeJsonPretty(msg_container, msg_buff);

    return String(msg_buff);
}


String MessagePacker::message(const char* code, const char* type){
  DynamicJsonDocument err(150);
  JsonObject message = err.to<JsonObject>();
  char err_buff[20];
  message[type] = code;
  serializeJson(message, err_buff);
  return String(err_buff); 
}