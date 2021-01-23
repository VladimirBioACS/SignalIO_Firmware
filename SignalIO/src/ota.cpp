#include "ota.h"

void OTA::update(){

    WiFi.mode(WIFI_STA);
    WiFi.begin();

    Serial.printf("OTA Begin\n");
    Serial.printf("Use your local IP adress to udate board: ");
    Serial.println(WiFi.localIP());

    //ArduinoOTA.setHostname(creds.user.c_str());
    //ArduinoOTA.setPassword((const char*)creds.password.c_str());
    ArduinoOTA.setPort(port);
    // Serial.println(creds.password.c_str());

    ArduinoOTA.onStart([](){
        String type;
        if(ArduinoOTA.getCommand() == U_FLASH){
            type = "scetch";
        }else
        {
            type = "filesystem";
        }
        if(type == "filesystem"){
            SPIFFS.end();
        }
        Serial.println("Start updating" + type);
    });

    ArduinoOTA.onEnd([](){

        Serial.printf("\nEnd updating\n");
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total){
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
        // digitalWrite(SIGNAL_LED, HIGH);
        // delay(10);
        // digitalWrite(SIGNAL_LED, LOW);
        // delay(10);
    });

    ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
    });
    
    ArduinoOTA.begin();

    //while(1){
    // ArduinoOTA.handle();
        //yield();
        //vTaskDelay(10 / portTICK_PERIOD_MS);
    //}
}