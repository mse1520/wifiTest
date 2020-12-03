void lcdDisplay() {
  String humData = "HUM: " + String(hum) + "%";
  String temCData = "TEMC: " + String(temC) + "C";
  String temFData = "TEMF: " + String(temF) + "F";
  String hitCData = "HITC: " + String(hiC) + "C";
  String hitFData = "HITF: " + String(hiF) + "F";
  
  String pm10Data = "PM1.0: " + String(pm10) + "ug/m^3";
  String pm25Data = "PM2.5: " + String(pm25) + "ug/m^3";
  String pm100Data = "PM10: " + String(pm100) + "ug/m^3";

  String datas[8] = {humData, temCData, temFData, hitCData, hitFData, pm10Data, pm25Data, pm100Data};
  
  for(int i=0; i<8; i++) {
    int strLength = 16-datas[i].length();
    for(int j=0; j<strLength; j++) {
      datas[i] += " ";
    }
  }
  
  lcd.setCursor(0, 1);
  lcd.print(datas[lcdIndex]);
  lcdIndex++;
  if(lcdIndex > 7) lcdIndex = 0;
}
