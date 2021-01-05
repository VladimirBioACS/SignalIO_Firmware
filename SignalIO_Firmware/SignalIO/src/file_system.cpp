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