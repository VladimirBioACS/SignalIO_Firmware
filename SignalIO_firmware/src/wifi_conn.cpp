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
      Serial.println("Failed to connect to network...");
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