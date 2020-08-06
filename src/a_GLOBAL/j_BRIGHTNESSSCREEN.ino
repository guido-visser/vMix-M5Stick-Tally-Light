void showBrightnessScreen(){
  screen = 3;
  screenRotation = 3;
  M5.Lcd.setRotation(3);
  int percentage = 100;
  if(BRIGHTNESS == 7){
    percentage = 0;
  } else if(BRIGHTNESS == 8){
    percentage = 20;
  } else if(BRIGHTNESS == 9){
    percentage = 40;
  } else if(BRIGHTNESS == 10){
    percentage = 60;
  } else if(BRIGHTNESS == 11){
    percentage = 80;
  }
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
  M5.Lcd.print(percentage);
  M5.Lcd.println("%");
}

void updateBrightnessVar(){
  if(BRIGHTNESS == 12){
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
  }
  updateBrightness();
  showBrightnessScreen();
}
void updateBrightness(){
  M5.Axp.ScreenBreath(BRIGHTNESS);
  saveBrightness();
}
