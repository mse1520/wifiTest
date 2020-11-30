#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <FS.h>

/* Set these to your desired credentials. */
const String softAP_ssid = "ap_smartfinder_sensor";
const String softAP_password = "00000000";
IPAddress apIP(192, 168, 5, 1);
IPAddress netMsk(255, 255, 255, 0);

String stat_ssid="", stat_pass="";
int stat_ip[4], stat_gateway[4], stat_netMsk[4];
const int led = 2;
bool statMode = false;

ESP8266WebServer server(80);

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 1);
  Serial.begin(2000000);
  SPIFFS.begin();
  
  bool readMode = readConfig();
  if(readMode) 
    setupStation();
  else 
    setupAp();
  
  serverRoute();
}

void setupAp() {
  Serial.print("AP: ");  
  Serial.println(softAP_ssid);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(softAP_ssid, softAP_password);
  delay(500); // Without delay I've seen the IP address blank
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
}

void setupStation() {
  Serial.print("STA: ");  
  WiFi.mode(WIFI_STA);

  bool ipCheck = true;
  for(int i=0; i<4; i++) {
    if (isnan(stat_ip[i]) || isnan(stat_gateway[i]) || isnan(stat_netMsk[i])) {
      ipCheck = false;
      break; 
    }
  }
  
  if(ipCheck) {
    Serial.println("test");
    IPAddress ip(stat_ip[0], stat_ip[1], stat_ip[2], stat_ip[3]);
    IPAddress gateway(stat_gateway[0], stat_gateway[1], stat_gateway[2], stat_gateway[3]);
    IPAddress netMsk(stat_netMsk[0], stat_netMsk[1], stat_netMsk[2], stat_netMsk[3]);
    WiFi.config(ip, gateway, netMsk);
  }
  
  WiFi.begin(stat_ssid, stat_pass);

  int count = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    count++;
    if(count > 60) break;
  }

  if(WiFi.status() != WL_CONNECTED) {
    SPIFFS.remove("/store.json");
    ESP.reset();
  }
  else {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
}

void serverRoute() {
  server.on("/", handleRoot);
  server.on("/setIp", HTTP_GET, handleSetIp);
  server.on("/save", HTTP_GET, handleInputPage);
  server.on("/save", HTTP_POST, handleSave);
  server.on("/read", HTTP_GET, handleRead);
  server.on("/data", HTTP_GET, handleData);
  server.on("/scan", HTTP_GET, handleScan);
  server.on("/clear", HTTP_GET, handleClear);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
