#include "config_menu.h"

const char* auth_login = "Signalio";
const char* auth_password = "root1234";

AsyncWebServer server(80);

void config_menu(){
    server.on("/save", HTTP_POST, [](AsyncWebServerRequest *request){
        FileSystem filesys;
        StaticJsonDocument<1024> p;
        
        char param_buff[1024];

        int param_parsed = request -> params();
        Serial.println(param_parsed);

        for (size_t i = 0; i < param_parsed; i++)
        {
            AsyncWebParameter* parameter = request->getParam(i);
            p[parameter -> name()] = parameter -> value();
        }
        //serializeJson(p, Serial);
        serializeJson(p, param_buff);
        // Serial.println(param_buff);

        bool save_flag = filesys.write_file("/config.json", param_buff);
        if(!save_flag){
            request->send_P(501, "text/html", "Error.");
        }
        request -> send(SPIFFS, "/index.html"); // Should return OK message on success
        delay(3000);
        ESP.restart();
    });

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        if(!request->authenticate(auth_login, auth_password)){
            return request->requestAuthentication();
        }
        request->send(SPIFFS, "/index.html");
    });

    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/style.css", "text/css");
    });

    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/script.js", "text/javascript");
    });

    server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request){
        if(!request->authenticate(auth_login, auth_password)){
            return request->requestAuthentication();
        }
        request->send(SPIFFS, "/settings.html");
    });

    server.on("/info", HTTP_GET, [](AsyncWebServerRequest *request){
        if(!request->authenticate(auth_login, auth_password)){
            return request->requestAuthentication();
        }
        request->send(SPIFFS, "/info.html");
    });

    while(1){
        server.begin();
    }
}