#include "wifi_conn.h"
 
FileSystem fileSystem_wifi;


bool wifiConn::wifi_connect(){
  StaticJsonDocument<255> wifi_creds = fileSystem_wifi.get_config("/wifi_creds.json");
  const char* ssid =  wifi_creds["ssid"];     
  const char* password = wifi_creds["password"]; 

  int counter = 0;

  digitalWrite(SIGNAL_LED, HIGH);
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    Serial.print("Status: ");
    Serial.println(status);
    status = WiFi.begin(ssid, password);
    ++counter; 
    delay(10000);
    if(WL_MAX_ATTEMPT_CONNECTION == counter){
        break;
      }
  }
  if(status == WL_CONNECTED){
    digitalWrite(SIGNAL_LED, LOW);
    return true;
  }
  else{
    return false;
  }
}

void wifiConn::printCurrentNet(){
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  byte bssid[6];
  Serial.print("BSSID: ");
  Serial.print(bssid[5], HEX);
  Serial.print(":");
  Serial.print(bssid[4], HEX);
  Serial.print(":");
  Serial.print(bssid[3], HEX);
  Serial.print(":");
  Serial.print(bssid[2], HEX);
  Serial.print(":");
  Serial.print(bssid[1], HEX);
  Serial.print(":");
  Serial.println(bssid[0], HEX);
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);
}

void wifiConn::printWifiData(){
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);
}


void wifiConn::wifi_manager(){
  AsyncWebServer wifi_server(WIFI_MANAGER_PORT);
  WiFi.softAP(access_point_ssid, access_point_passwor);
  Serial.println("local wifi manager started");
  
  Serial.print("SoftAP name: ");
  Serial.println(access_point_ssid);

  IPAddress ip = WiFi.softAPIP();
  Serial.print("Use local ip to connect to Wi-Fi manager: ");
  Serial.println(ip);

  wifi_server.on("/wifi_save_creds", HTTP_POST, [](AsyncWebServerRequest *request){
    DynamicJsonDocument p_wifi(255);
    char wifi_param_buff[255];

    int param_parsed = request -> params();
    Serial.println(param_parsed);

    for(size_t i = 0; i < param_parsed; i++)
    {
        AsyncWebParameter* parameter = request->getParam(i);
        p_wifi[parameter -> name()] = parameter -> value();
    }
    serializeJson(p_wifi, wifi_param_buff);

    bool save_flag = fileSystem_wifi.write_file("/wifi_creds.json", wifi_param_buff);
    if(!save_flag){
        request->send_P(501, "text/html", "Error.");
        delay(5000);
        ESP.restart();
    }
    request -> send(SPIFFS, "/wifi.html"); // Should return OK message on success
    delay(3000);
    ESP.restart();
  });

  wifi_server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/wifi.html");
  });

  while(1){
    wifi_server.begin();
  }
}