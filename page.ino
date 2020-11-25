void handleRoot() {
  String s = ""; 
  s += "<meta charset='utf-8'/>";
  s += "<meta name='viewport' content='width=device-width, initial-scale=1.0'/>";
  
  s += "<style>";
  s += "  body{";
  s += "    text-align: center;";
  s += "  }";
  s += "  table{";
  s += "    margin: auto;";
  s += "  }";
  s += "</style>";
  
  s=s+"<table><tr><th align='left'>검색된 와이파이</th></tr>";
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n > 0) {
    for (int i = 0; i < n; i++) {
      int wifiS = round(((WiFi.RSSI(i) + 90) / 60.0) * 100);
      if(wifiS > 100) wifiS =100;
      if(wifiS >= 0){
        s += "<tr><td><a>" + WiFi.SSID(i) + "</a></td>";
        s += "<td>" + String((WiFi.encryptionType(i) == ENC_TYPE_NONE)?"Unlock":"Lock") + "</td>"; 
        s += "<td>" + String(wifiS) + "%</td></tr>";
      }
    }
  } else {
    s=s+"<tr><td>No WLAN found</td></tr>";
  }
  s=s+"</table>";
  
  s += "<form action='/save' method='post'>";
  s += "  <input type='text' name='ssid' placeholder='SSID'><br>";
  s += "  <input type='password' name='pass' placeholder='password'><br>";
  s += "  <input type='submit' value='저장'>";
  s += "</form>";
  server.send(200, "text/html", s);
}

void handleSave() {
  String s = "";
  s += "<meta charset='utf-8'/>";
  s += "<meta name='viewport' content='width=device-width, initial-scale=1.0'/>";
  s += server.arg("ssid");
  s += server.arg("pass");
  server.send(200, "text/html", s);
}

void handleData() {
  String s = "{\"sensor1\":\"message\",\"sensor\":123}";
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", s);
}

void handleScan() {
  int scan = WiFi.scanNetworks();
  String s="[";
  
  for(int i=0; i<scan; i++) {
    int wifiS = round(((WiFi.RSSI(i) + 90) / 60.0) * 100);
    if(wifiS > 100) wifiS = 100;
    if(wifiS >= 0) {
      s += "{\"ssid\":\"" + WiFi.SSID(i) + "\",";
      s += "\"lock\":" + String((WiFi.encryptionType(i) == ENC_TYPE_NONE)?"false":"true") + ",";
      s += "\"signal\":\"" + String(wifiS) + "%\"}";
      if(i == scan-1) s += "]";
      else s += ",";
    }
  }
  
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", s);
}

void handleNotFound() {
  digitalWrite(led, 1);
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
  digitalWrite(led, 0);
}
