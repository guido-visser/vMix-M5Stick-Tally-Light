void showNetworkScreen() {
  Serial.println("Showing Network screen");
  screen = 1;
  cls();
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.println("SSID:");
  M5.Lcd.println(WIFI_SSID);
  M5.Lcd.println();
  M5.Lcd.println("IP Address:");
  M5.Lcd.println(WiFi.localIP());
}
