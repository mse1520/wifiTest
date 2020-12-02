#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <DHT.h>
#include <SimpleTimer.h>
#include <pm2008_i2c.h>

SimpleTimer timer;

#define LED D4
#define DHTPIN D3
//#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

/* AP */
const String softAP_ssid = "ap_smartfinder_sensor";
const String softAP_password = "00000000";
IPAddress apIP(192, 168, 5, 1);
IPAddress netMsk(255, 255, 255, 0);

/* Station */
String stat_ssid="", stat_pass="";
int stat_ip[4], stat_gateway[4], stat_netMsk[4];

/* Sensor */
DHT dht(DHTPIN, DHTTYPE);
PM2008_I2C pm2008_i2c;
float hum, temC, temF, hiF, hiC;
int pm10, pm25, pm100;

ESP8266WebServer server(80);

void setup(void) {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, 1);
  
  Serial.begin(2000000);
  
  dht.begin();
  pm2008_i2c.begin();
  pm2008_i2c.command();

  timer.setInterval(2000, runSensor);

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

  for(int i=0; i<3; i++) {
    digitalWrite(LED, 0);
    delay(500);
    digitalWrite(LED, 1);
    delay(500);
  }
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
    
    for(int i=0; i<5; i++) {
      digitalWrite(LED, 0);
      delay(500);
      digitalWrite(LED, 1);
      delay(500);
    }
  }
}

void serverRoute() {
  server.on("/", handleMainPage);
  server.on("/setIp", HTTP_GET, handleSetIpPage);
  server.on("/setIp", HTTP_POST, handleSetIp);
  server.on("/save", HTTP_GET, handleSavePage);
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
  timer.run();
}
