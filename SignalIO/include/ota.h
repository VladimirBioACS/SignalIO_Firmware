#include "Arduino.h"
#include "ArduinoOTA.h"
#include "WiFi.h"
#include "file_system.h"

#ifndef OTA_H
#define OTA_H

class OTA
{
private:
    int port = 8266;

public:
    void update(void);
};

#endif