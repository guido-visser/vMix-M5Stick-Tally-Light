void showAPScreen()
{
    Serial.println("Showing Access Point Screen");
    screen = 0;
    cls();

    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.setCursor(10, 20);
    M5.Lcd.println("Unable to connect to WiFi");
    M5.Lcd.println();
    M5.Lcd.println("Please connect to:");
    M5.Lcd.println();
    M5.Lcd.println("SSID: vMix-M5tack-Tally");
    M5.Lcd.println("Pwd: 12345678");
    M5.Lcd.println();
    M5.Lcd.println("Open http://192.168.4.1 \n in a browser and \n configure your WiFi");
    M5.Lcd.println();
}
