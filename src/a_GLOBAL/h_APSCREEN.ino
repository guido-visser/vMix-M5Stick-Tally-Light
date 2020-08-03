void showAPScreen()
{
    resetScreen();
    Serial.println("Showing Access Point Screen");
    screen = 0;
    
    M5.Lcd.println("Unable to connect to WiFi");
    M5.Lcd.println();
    M5.Lcd.println("Please connect to:");
    M5.Lcd.println();
    M5.Lcd.println("SSID: vMix-M5tick-Tally");
    M5.Lcd.println("Pwd: 12345678");
    M5.Lcd.println();
    M5.Lcd.println("Open http://192.168.4.1");
    M5.Lcd.println("in a browser to");
    M5.Lcd.println("configure the settings");
}
