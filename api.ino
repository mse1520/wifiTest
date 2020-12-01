void handleRoot() {
  String s = "test"; 
  server.send(200, "text/html", s);
}

void handleSave() {
  String ssid = server.arg("ssid");
  String pass = server.arg("pass");
  String s = "failure";
  
  if (ssid != "" && pass != "") {
    String storeMsg = "{\"ssid\":\"" + ssid + "\",\"pass\":\"" + pass + "\"}";
    bool writeResult = writeConfig(storeMsg);
    if(writeResult) s = "success";
  }
  
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", s);

//  if(s == "success") ESP.reset();
}

void handleSetIp() {
  String ip = server.arg("ip");
  String gateway = server.arg("gateway");
  String netMsk = server.arg("netMsk");
  String result = "failure";

  if (ip != "" && gateway != "" && netMsk != "" && stat_ssid != "" && stat_pass != "") {
    String storeMsg = "{\"ssid\":\"" + stat_ssid + "\",\"pass\":\"" + stat_pass + "\",";
    storeMsg += "\"ip\":" + ip + ",\"gateway\":" + gateway + ",\"netMsk\":" + netMsk + "}";
    bool writeResult = writeConfig(storeMsg);
    if(writeResult) result = "success";
  }
  
  server.send(200, "text/plain", result);
}

void handleRead() {
  String s = "failure";
  bool result = readConfig();
  if(result) s = "success";
  server.send(200, "text/plain", s);
}

void handleClear() {
  String s = "test";
  SPIFFS.remove("/store.json");
//  SPIFFS.format();
  server.send(200, "text/plain", s);
}

void handleData() {
  String s = "[";
  s += "{\"sensor\":\"Humidity\",\"data\":" + String(hum) + "},";
  s += "{\"sensor\":\"Celsius\",\"data\":" + String(temC) +"},";
  s += "{\"sensor\":\"Fahrenheit\",\"data\":" + String(temF) +"},";
  s += "{\"sensor\":\"Perceived Celsius\",\"data\":" + String(hiC) +"},";
  s += "{\"sensor\":\"Perceived Fahrenheit\",\"data\":" + String(hiF) +"}";
  s += "]";
  
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", s);
}

void handleScan() {
  int scan = WiFi.scanNetworks();
  String s="[";
  
  for(int i=0; i<scan; i++) {
    int wifiS = round(((WiFi.RSSI(i) + 90) / 60.0) * 100);
    if(wifiS > 100) wifiS = 100;
    if(wifiS > 0) {
      s += "{\"ssid\":\"" + WiFi.SSID(i) + "\",";
      s += "\"lock\":" + String((WiFi.encryptionType(i) == ENC_TYPE_NONE)?"false":"true") + ",";
      s += "\"signal\":" + String(wifiS) + "}";
      if(i == scan-1) s += "]";
      else s += ",";
    }
  }
  
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", s);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
