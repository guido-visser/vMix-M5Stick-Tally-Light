void connectToWifi()
{
  Serial.println();
  Serial.println("------------");
  Serial.println("Connecting to WiFi");
  Serial.print("SSID: ");
  Serial.println(WIFI_SSID);
  Serial.print("Passphrase: ");
  Serial.println(WIFI_PASS);

  int timeout = 15;

  M5.Lcd.println();
  M5.Lcd.println();
  M5.Lcd.setCursor(25, 60);
  M5.Lcd.println("Waiting for WiFi");

  WiFi.mode(WIFI_STA);
  WiFi.begin(&(WIFI_SSID[0]), &(WIFI_PASS[0]));

  Serial.print("Waiting for connection.");
  while (WiFi.status() != WL_CONNECTED and timeout > 0)
  {
    delay(1000);
    timeout--;
    Serial.print(".");
    M5.Lcd.print(".");
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Success!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Device name: ");
    Serial.println(deviceName);
    Serial.println("------------");
  }
  else
  {
    if (WiFi.status() == WL_IDLE_STATUS)
      Serial.println("Idle");
    else if (WiFi.status() == WL_NO_SSID_AVAIL)
      Serial.println("No SSID Available");
    else if (WiFi.status() == WL_SCAN_COMPLETED)
      Serial.println("Scan Completed");
    else if (WiFi.status() == WL_CONNECT_FAILED)
      Serial.println("Connection Failed");
    else if (WiFi.status() == WL_CONNECTION_LOST)
      Serial.println("Connection Lost");
    else if (WiFi.status() == WL_DISCONNECTED)
      Serial.println("Disconnected");
    else
      Serial.println("Unknown Failure");

    Serial.println("------------");
    //apStart();
  }
}

void startWiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(&(WIFI_SSID[0]), &(WIFI_PASS[0]));

    // We start by connecting to a WiFi network

    //WiFi.begin(&(WIFI_SSID[0]), &(WIFI_PASS[0]));
    //WiFi.softAP(ssid, password);

    M5.Lcd.println();
    M5.Lcd.setCursor(25, 60);
    M5.Lcd.println("Waiting for WiFi...");

    //while (WiFi.status() != WL_CONNECTED) {
    int tries = 0;
    boolean wifi_connected = true;

    while (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        M5.Lcd.print(".");
        delay(1000);
        tries++;
        if (tries > 10)
        {
            tries = 0;
            Serial.println("Wifi connection failed, start local wifi");
            wifi_connected = false;
            startLocalWiFi();
            break;
        }
    }

    if (wifi_connected == false)
    {
        startLocalWiFi();
    }
    else
    {
        cls();
        M5.Lcd.println("WiFi connected");
        M5.Lcd.println("IP address: ");
        M5.Lcd.print(WiFi.localIP());
        startServer();
        connectTovMix();
    }
}

// This starts the M5Stack as a WiFi Access Point so you can configure it
void startLocalWiFi()
{
    WiFi.mode(WIFI_AP);
    WiFi.softAP("vMix-M5Stack-Tally", "12345678");
    showAPScreen();
    startServer();
}
