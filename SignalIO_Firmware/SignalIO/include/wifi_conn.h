#include <Arduino.h>
#include <SPI.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "file_system.h"

#ifndef WIFI_CONN_H
#define WIFI_CONN_H
#define WIFI_MANAGER_PORT 80
#define WL_MAX_ATTEMPT_CONNECTION 3

class wifiConn
{
private:
    int status = WL_IDLE_STATUS;
    const char* ssid =  "Polaric"; //"Pautina_1";     
    const char* password = "polaric20"; //"123qweasdzxc";

    const char* access_point_ssid = "SignalIO_Dev_Board";
    const char* access_point_passwor = "root1234";

    String wifi_manager_page = R"=====(
        <!DOCTYPE html>
        <html lang="en" dir="ltr">
        <head>
            <meta charset="utf-8">
            <title>SignalIO Config</title>
            <style media="screen">
            :root {
            --main-bg-color: #1b262c;
            --main-text-color: #fff;
            }
            .btn-default,
            .btn-default:hover,
            .btn-default:focus {
            color: var(--main-bg-color);
            text-shadow: none;
            background-color: var(--main-text-color);
            border: 1px solid var(--main-text-color);
            font-size:15px;
            width:50px;
            height:30px;
            border-radius:5px;
            }
            body {
            font-family:arial;
            background-color: var(--main-bg-color);
            color: var(--main-text-color);
            text-align: center;
            text-shadow: 0 1px 3px rgba(0,0,0,.5);
            }
            .site-wrapper {
            display: table;
            width: 100%;
            background-color: var(--main-bg-color);
            height: 100%;
            min-height: 100%;
            margin-top:100px;
            }
            .site-wrapper-inner {
            display: table-cell;
            vertical-align: top;
            }
            .cover-container {
            margin-right: auto;
            margin-left: auto;
            }
            .inner {
            padding: 30px;
            }

            .container{
            margin-right: auto;
            margin-left: auto;
            }
            input{
            border-radius:2px;
            border-style:none;
            margin:15px;
            height:25px;
            }
            </style>
        </head>
        <body>
                <div class="site-wrapper">
                <div class="site-wrapper-inner">
                    <div class="container">
                    <br>
                    <div class="inner cover">
                        <h1 class="cover-heading"> <span><i class="fas fa-cogs"></i></span> <b>Wi-Fi Config Page</b></h1>
                        <br>
                        <form class="" action="index.html" method="post">
                        <b>Network SSID</b>
                        <br>
                        <input name="wifi_ssid" type="text" class="form-control" placeholder="SSID" required>
                        <br>
                        <b>Network Password</b>
                        <br>
                        <input name="wifi_password" type="password" class="form-control" placeholder="Password" required>
                        <br>
                        <p class="lead">
                        <button type="submit" class=" btn-default">Save</button>
                        </p>
                        <p class="lead">
                        <button type="submit" class=" btn-default">Scan</button>
                        </p>
                        </form>
                        </div>
                        </div>
                    </div>
                </div>
        </body>
        </html>
    )=====";


public:
    bool wifi_connect();
    void printCurrentNet();
    void printWifiData();
    void wifi_manager(void);
};

#endif