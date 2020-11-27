bool writeConfig(String storeMsg) {
  bool result = false;
  File writeFile = SPIFFS.open("/store.json", "w");
  
  if (writeFile) {
    writeFile.println(storeMsg);
    writeFile.close();
    result = true;
  }
  
  return result;
}

bool readConfig() {
  bool result = false;

  File readFile = SPIFFS.open("/store.json", "r");   
  if (readFile) {
    String storeFile = readFile.readStringUntil('}') + "}";
    
    StaticJsonDocument<200> doc;
    DeserializationError Error = deserializeJson(doc, storeFile);
    if(!Error) {
      stat_ssid = doc["ssid"];
      stat_pass = doc["pass"];
      Serial.println(storeFile);
      Serial.println(ssid);
      Serial.println(pass);
      result = true; 
    }
    
    readFile.close();
  }
  
  return result;
}
