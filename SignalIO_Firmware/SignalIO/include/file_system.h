#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <Arduino.h>
#include <FS.h>
#include <SPIFFS.h>
#include "json_lib.h"

class FileSystem{
    private:
        const int buff_size = 1024;

    public:
        bool read_file(const char* path);
        bool write_file(const char* path, char buff[]);
        bool config_reset(const char* path, int key);
        
        StaticJsonDocument<1024> get_config(const char* path);
};

#endif