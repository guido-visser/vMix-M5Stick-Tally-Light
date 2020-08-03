void showNetworkScreen() {
  Serial.println("Showing Network screen");
  resetScreen();
  screen = 1;
  M5.Lcd.println("SSID:");
  M5.Lcd.println(WIFI_SSID);
  M5.Lcd.println();
  M5.Lcd.println("IP Address:");
  M5.Lcd.println(WiFi.localIP());
}
