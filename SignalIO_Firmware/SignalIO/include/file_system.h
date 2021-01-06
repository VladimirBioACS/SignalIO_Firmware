#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <Arduino.h>
#include <FS.h>
#include <SPIFFS.h>
#include "json_lib.h"

#ifndef CONFIG_LITTLEFS_FOR_IDF_3_2
 #include <time.h>
#endif

#define FORMAT_LITTLEFS_IF_FAILED true

class FileSystem{
    private:
        const int buff_size = 1024;

    public:
        bool read_file(const char* path);
        bool write_file(const char* path, char buff[]);
        bool test_fs(void);
        StaticJsonDocument<1024> get_config(const char* path);
};

#endif