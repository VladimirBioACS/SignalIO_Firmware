#include "config_menu.h"

AsyncWebServer server(80);
FileSystem fileSystem_config;

const char* creds_path = "/auth_creds.json";

const char* auth_login;
const char* auth_password;

bool creds_validator(StaticJsonDocument<255> auth_cred){
    if(auth_cred["old_username"] == auth_login && auth_cred["old_password"] == auth_password){
        return true;
    }
    return false;
}


void config_menu(){
    StaticJsonDocument<255> confs = fileSystem_config.get_config(creds_path);

    auth_login = confs["username"];
    auth_password = confs["password"];

    server.on("/save", HTTP_POST, [](AsyncWebServerRequest *request){
        if(!request->authenticate(auth_login, auth_password)){
            return request->requestAuthentication();
        }
        DynamicJsonDocument p(1024);
        char param_buff[1024];

        int param_parsed = request -> params();
        Serial.println(param_parsed);

        for (size_t i = 0; i < param_parsed; i++)
        {
            AsyncWebParameter* parameter = request->getParam(i);
            p[parameter -> name()] = parameter -> value();
        }
        
        p["config_flag"] = 0;
        serializeJson(p, param_buff);

        bool save_flag = fileSystem_config.write_file("/config.json", param_buff);
        if(!save_flag){
            request->send_P(501, "text/html", "Error.");
            delay(5000);
            ESP.restart();
        }
        request -> send(SPIFFS, "/index.html"); // Should return OK message on success
        delay(3000);
        ESP.restart();
    });

    //TODO (test)
    server.on("/change-auth-creds", HTTP_POST, [](AsyncWebServerRequest *request){
        if(!request->authenticate(auth_login, auth_password)){
            return request->requestAuthentication();
        }
        DynamicJsonDocument auth_p(255);
        char auth_param_buff[255];

        int auth_param_parsed = request -> params();
        for (size_t i = 0; i < auth_param_parsed; i++)
        {
            AsyncWebParameter* parameter = request->getParam(i);
            auth_p[parameter -> name()] = parameter -> value();
        }
        serializeJson(auth_p, auth_param_buff);
        Serial.println("\n");
        bool creds_validation_flag = creds_validator(auth_p);
        if(creds_validation_flag){
            bool update_creds_flag = fileSystem_config.write_file(creds_path, auth_param_buff);
            if(!update_creds_flag){
                Serial.println("ERROR. Auth creds not updated"); // should be replaced by message page
            }
        }
        request->send(SPIFFS, "/index.html");
    });

    // TODO (test)
    server.on("/reboot-device", HTTP_GET, [](AsyncWebServerRequest *request){
        if(!request->authenticate(auth_login, auth_password)){
            return request->requestAuthentication();
        }
        //request->send(SPIFFS, "/index.html");
        Serial.println("Board will restart after 1 sec");
        delay(1000);
        ESP.restart();
    });


    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        if(!request->authenticate(auth_login, auth_password)){
            return request->requestAuthentication();
        }
        request->send(SPIFFS, "/index.html");
    });


    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
        if(!request->authenticate(auth_login, auth_password)){
            return request->requestAuthentication();
        }
        request->send(SPIFFS, "/style.css", "text/css");
    });


    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
        if(!request->authenticate(auth_login, auth_password)){
            return request->requestAuthentication();
        }
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