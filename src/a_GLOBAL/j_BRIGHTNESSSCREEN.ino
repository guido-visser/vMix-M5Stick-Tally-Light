void showBrightnessScreen(){
  screen = 3;
  screenRotation = 3;
  M5.Lcd.setRotation(3);

  resetScreen();
  ledToggle(false);
  M5.Lcd.fillScreen(WHITE);
  M5.Lcd.setTextColor(BLACK, WHITE);

  // ---------- TITLE ----------
  M5.Lcd.setTextSize(2);
  String title1 = "ADJUST";
  String title2 = "BRIGHTNESS";

  int w1 = M5.Lcd.textWidth(title1);
  int w2 = M5.Lcd.textWidth(title2);
  int cx = M5.Lcd.width() / 2;

  M5.Lcd.setCursor(cx - w1 / 2, 10);
  M5.Lcd.println(title1);

  M5.Lcd.setCursor(cx - w2 / 2, 32);
  M5.Lcd.println(title2);

  // ---------- ARROW ----------
  int arrowX = cx;
  int arrowTop = 60;

  M5.Lcd.drawLine(arrowX, arrowTop, arrowX, arrowTop + 20, BLACK);
  M5.Lcd.drawTriangle(
    arrowX, arrowTop + 30,
    arrowX - 10, arrowTop + 20,
    arrowX + 10, arrowTop + 20,
    BLACK
  );
  M5.Lcd.fillTriangle(
    arrowX, arrowTop + 30,
    arrowX - 10, arrowTop + 20,
    arrowX + 10, arrowTop + 20,
    BLACK
  );

  // ---------- PERCENTAGE ----------
  int percentage = brightnessPctFromVar(BRIGHTNESS);
  String pct = String(percentage) + "%";

  M5.Lcd.setTextSize(3);
  int pw = M5.Lcd.textWidth(pct);

  M5.Lcd.setCursor(cx - pw / 2, arrowTop + 40);
  M5.Lcd.print(pct);
}

void updateBrightnessVar(){
  if (BRIGHTNESS == 12) {
    BRIGHTNESS = 7;
  } else if (BRIGHTNESS == 7) {
    BRIGHTNESS = 8;
  } else if (BRIGHTNESS == 8) {
    BRIGHTNESS = 9;
  } else if (BRIGHTNESS == 9) {
    BRIGHTNESS = 10;
  } else if (BRIGHTNESS == 10) {
    BRIGHTNESS = 11;
  } else if (BRIGHTNESS == 11) {
    BRIGHTNESS = 12;
  } else {
    BRIGHTNESS = 12;
  }

  updateBrightness();
  ledToggle(false);
  showBrightnessScreen();
}

void updateBrightness(){
  #if C_PLUS == 0 || C_PLUS == 1
    M5.Axp.ScreenBreath((BRIGHTNESS - 6) * 16);

  #elif C_PLUS == 2
    int pct = brightnessPctFromVar(BRIGHTNESS);
    pct = constrain(pct, 10, 100);
    uint8_t b = (uint8_t)(pct * 255 / 100);
    M5.Display.setBrightness(b);
  #endif

  saveBrightness();
}
