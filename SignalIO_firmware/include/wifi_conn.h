#include <Arduino.h>
#include <SPI.h>
#include "WiFi.h"

#ifndef WIFI_CONN_H
#define WIFI_CONN_H

class wifiConn
{
private:
    int status = WL_IDLE_STATUS;
    const char* ssid = "Pautina_1";     
    const char* password = "123qweasdzxc"; 

public:
    bool wifi_connect();
    void printCurrentNet();
    void printWifiData();
};

#endif