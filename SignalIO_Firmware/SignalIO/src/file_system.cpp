#include "file_system.h"


bool FileSystem::read_file(const char* path){
    File file = SPIFFS.open(path, "r");
    size_t size = file.size();
    
    if(!file){
        Serial.println("Error opening file: " + String(path));
        return false;
    }
    Serial.println("Config file exist: " + String(path));
    Serial.printf("File size: %i (bytes)\n", size);
    while(file.available()){
        Serial.write(file.read());
    }
    Serial.println(""); // \n
    file.close();
    return true;
}

StaticJsonDocument<1024> FileSystem::get_config(const char* path){

  DynamicJsonDocument json_config(1024);

  File configFile = SPIFFS.open(path, "r");
  if(!configFile){
    Serial.println("failed to load JSON config");
    configFile.close();
    return json_config;
  }
  size_t size = configFile.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);

  //serializeJson(json_config, buf);
  //deserializeJson(buf, json_config);

  return json_config;
}

bool FileSystem::test_fs(){

}


bool FileSystem::write_file(const char* path, char buff[]){
    File f = SPIFFS.open(path, "w");
    size_t size = f.size();

    if(!f){
        f.close();
        Serial.println("File not found"); // debug msg
        return false;
    }
    if (size > buff_size)
    {
        f.close();
        Serial.println("File too large"); //debug msg
        return false;
    }

    if(f.print(buff)){
        Serial.println("File written successfully"); //debug msg
        return true;
    }

    Serial.println("Write failed"); //debug msg
    return false;   
}