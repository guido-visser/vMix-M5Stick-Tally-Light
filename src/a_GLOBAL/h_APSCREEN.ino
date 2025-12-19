void showAPScreen()
{
  resetScreen();
  screen = 4;

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE, BLACK);

  int cx = M5.Lcd.width() / 2;
  int y = 6;

  // ---- TITLE (small) ----
  M5.Lcd.setTextSize(1);
  String title = "NO WIFI";
  M5.Lcd.setCursor(cx - M5.Lcd.textWidth(title) / 2, y);
  M5.Lcd.println(title);

  y += 14;

  // ---- SUBTEKST ----
  M5.Lcd.setTextSize(1);
  String sub = "Unable to connect to WiFi";
  M5.Lcd.setCursor(cx - M5.Lcd.textWidth(sub) / 2, y);
  M5.Lcd.println(sub);

  y += 18;

  // ---- SSID LABEL ----
  M5.Lcd.setTextSize(1);
  String ssidLbl = "SSID";
  M5.Lcd.setCursor(cx - M5.Lcd.textWidth(ssidLbl) / 2, y);
  M5.Lcd.println(ssidLbl);

  y += 12;

  // ---- SSID VALUE ----
  M5.Lcd.setTextSize(2);
  String ssidVal = "vMix-M5Stick";
  M5.Lcd.setCursor(cx - M5.Lcd.textWidth(ssidVal) / 2, y);
  M5.Lcd.println(ssidVal);

  y += 22;

  // ---- PASSWORD ----
  M5.Lcd.setTextSize(2);
  String pwd = "PWD: 12345678";
  M5.Lcd.setCursor(cx - M5.Lcd.textWidth(pwd) / 2, y);
  M5.Lcd.println(pwd);

  y += 22;

  // ---- BROWSER INFO ----
  M5.Lcd.setTextSize(1);
  String open = "Open in browser:";
  M5.Lcd.setCursor(cx - M5.Lcd.textWidth(open) / 2, y);
  M5.Lcd.println(open);

  y += 12;

  // ---- IP ----
  M5.Lcd.setTextSize(2);
  String url = "192.168.4.1";
  M5.Lcd.setCursor(cx - M5.Lcd.textWidth(url) / 2, y);
  M5.Lcd.println(url);
}