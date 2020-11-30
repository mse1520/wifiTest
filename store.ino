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

    StaticJsonDocument<500> doc;
    DeserializationError Error = deserializeJson(doc, storeFile);
    if(!Error) {
      stat_ssid = doc["ssid"].as<String>();
      stat_pass = doc["pass"].as<String>();

      for(int i=0; i<4; i++) {
        stat_ip[i] = doc["ip"][i].as<int>();
        stat_gateway[i] = doc["gateway"][i].as<int>();
        stat_netMsk[i] = doc["netMsk"][i].as<int>();
      }

      result = true;
    }
    
    readFile.close();
  }
  
  return result;
}
