#include "wifi_conn.h"
 

bool wifiConn::wifi_connect(){
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
  WiFiServer wifiServer(WIFI_MANAGER_PORT);
  WiFi.softAP(access_point_ssid, access_point_passwor);
  Serial.println("local wifi manager started");
  IPAddress ip = WiFi.softAPIP();
  Serial.println(ip);

  wifiServer.begin();

  while(1){
    WiFiClient client = wifiServer.available();
    if(client){
      Serial.println("New client is connected");
      client.println("HTTP/1.1 200 OK");
      client.println("Content-type:text/html");
      client.println("Connection: close");
      client.println();
      client.println(wifi_manager_page);
      client.println();
      client.stop();
      Serial.println("Client disconnected");
    }
  }

}