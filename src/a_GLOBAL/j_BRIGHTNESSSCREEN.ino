void showBrightnessScreen(){
  screen = 3;
  resetScreen();
  M5.Lcd.setTextSize(2);
  digitalWrite(LED_BUILTIN, HIGH);
  M5.Lcd.fillScreen(WHITE);
  M5.Lcd.setTextColor(BLACK, WHITE);
  M5.Lcd.setCursor(45, 5);
  M5.Lcd.println("ADJUST");
  M5.Lcd.setCursor(23, 23);
  M5.Lcd.println("BRIGHTNESS");
  M5.Lcd.drawLine(80, 51, 80, 70, BLACK);
  M5.Lcd.drawTriangle(80, 80, 70, 70, 90, 70, BLACK);
  M5.Lcd.fillTriangle(80, 80, 70, 70, 90, 70, BLACK);
  M5.Lcd.setCursor(10, 50);
  M5.Lcd.print(BRIGHTNESS);
  M5.Lcd.print("/15");
}

void updateBrightnessVar(){
  if(BRIGHTNESS == 15){
    BRIGHTNESS = 7;
  } else if(BRIGHTNESS == 7){
    BRIGHTNESS = 8;
  } else if(BRIGHTNESS == 8){
    BRIGHTNESS = 9;
  } else if(BRIGHTNESS == 9){
    BRIGHTNESS = 10;
  } else if(BRIGHTNESS == 10){
    BRIGHTNESS = 11;
  } else if(BRIGHTNESS == 11){
    BRIGHTNESS = 12;
  } else if(BRIGHTNESS == 12){
    BRIGHTNESS = 13;
  } else if(BRIGHTNESS == 13){
    BRIGHTNESS = 14;
  } else if(BRIGHTNESS == 14){
    BRIGHTNESS = 15;
  }
  updateBrightness();
  showBrightnessScreen();
}
void updateBrightness(){
  M5.Axp.ScreenBreath(BRIGHTNESS);
  saveBrightness();
}
