void showResetScreen(bool reset){
  screen = 5;
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(WHITE, BLACK);
  if(reset){
    cls();
    resetSettings();
  } else {
    cls();
    M5.Lcd.println("Press BTN39 10 times to reset!");
    M5.Lcd.println();
    M5.Lcd.print(resetCount);
    M5.Lcd.print("/");
    M5.Lcd.print("10");
  }
}
