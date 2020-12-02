void runSensor() {
  getDht();
  getPm2008m();
}

void getDht() {
  hum = dht.readHumidity();
  temC = dht.readTemperature();
  temF = dht.readTemperature(true);

  if (isnan(hum) || isnan(temC) || isnan(temF)) {
    Serial.println(F("Failed to read from DHT sensor!"));
  }
  else {
    // 체감온도
    hiF = dht.computeHeatIndex(temF, hum);
    hiC = dht.computeHeatIndex(temC, hum, false);

//    Serial.print(F("Humidity: "));
//    Serial.print(hum);
//    Serial.println(F("%"));
//    
//    Serial.print(F("Temperature: "));
//    Serial.print(temC);
//    Serial.print(F("°C, "));
//    Serial.print(temF);
//    Serial.println(F("°F"));
//    
//    Serial.print(F("Heat index: "));
//    Serial.print(hiC);
//    Serial.print(F("°C, "));
//    Serial.print(hiF);
//    Serial.println(F("°F")); 
  }
}

void getPm2008m() {
  uint8_t ret = pm2008_i2c.read();
  if (ret == 0) {
    pm10 = pm2008_i2c.pm1p0_grimm;
    pm25 = pm2008_i2c.pm2p5_grimm;
    pm100 = pm2008_i2c.pm10_grimm;
    
//    Serial.print("PM 1.0 (GRIMM) : ");
//    Serial.println(pm2008_i2c.pm1p0_grimm);
//    Serial.print("PM 2.5 (GRIMM) : ");
//    Serial.println(pm2008_i2c.pm2p5_grimm);
//    Serial.print("PM 10 (GRIMM) : ");
//    Serial.println(pm2008_i2c.pm10_grimm);
  }
}
