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
String statIp="", statGateway="", statNetMsk="";
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
//  String statIp, String statGateway, String statNetMsk
//  IPAddress ip(192, 168, 0, 101);
//  IPAddress gateway(117, 16, 177, 126);
//  IPAddress netMsk(255, 255, 255, 0);
//  WiFi.config(ip, gateway, subnet);
  WiFi.begin(stat_ssid, stat_pass);

  int count = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    count++;
    if(count > 60) break; 
  }
  if(WiFi.status() != )
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void serverRoute() {
  server.on("/", handleRoot);
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
