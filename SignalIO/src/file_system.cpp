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
    file.close();
    return true;
}


bool FileSystem::test_fs(){

}


bool FileSystem::write_file(){

}