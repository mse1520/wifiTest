#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/* Set these to your desired credentials. */
const String softAP_ssid = "ap_synergy_sensor";
const String softAP_password = "00000000";
/* Soft AP network parameters */
IPAddress apIP(192, 168, 5, 1);
IPAddress netMsk(255, 255, 255, 0);

ESP8266WebServer server(80);

const int led = 2;

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 1);
  
  Serial.begin(2000000);

  setupAp();

  server.on("/", handleRoot);
  server.on("/save", handleSave);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void setupAp() {
  Serial.println(softAP_ssid);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(softAP_ssid, softAP_password);
  delay(500); // Without delay I've seen the IP address blank
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
}

void loop(void) {
  server.handleClient();
}
