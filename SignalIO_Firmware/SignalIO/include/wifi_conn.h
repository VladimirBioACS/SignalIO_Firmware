#include <Arduino.h>
#include <SPI.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "json_lib.h"
#include "file_system.h"

#ifndef WIFI_CONN_H
#define WIFI_CONN_H
#define WIFI_MANAGER_PORT 80
#define WL_MAX_ATTEMPT_CONNECTION 3

class wifiConn
{
private:
    int status = WL_IDLE_STATUS;

    const char* access_point_ssid = "SignalIO_Dev_Board";
    const char* access_point_passwor = "root1234";
    

public:
    bool wifi_connect();
    void printCurrentNet();
    void printWifiData();
    void wifi_manager(void);
};

#endif