WiFiClient client;
WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)

PinButton btnM5(37);

char currentState = -1;
char screen = 0;

void setup()
{
  Serial.begin(115200);
  M5.begin();
  delay(10);
  M5.Lcd.setRotation(3);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  M5.Lcd.setCursor(20, 20);
  M5.Lcd.println("vMix M5Stick-C Tally");
  M5.Lcd.setCursor(35, 40);
  M5.Lcd.println("by Guido Visser");
  delay(2000);

  // We start by connecting to a WiFi network
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  M5.Lcd.println();
  M5.Lcd.println();
  M5.Lcd.setCursor(25, 60);
  M5.Lcd.println("Waiting for WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    M5.Lcd.print(".");
    delay(500);
  }
  cls();
  M5.Lcd.println("WiFi connected");
  M5.Lcd.println("IP address: ");
  M5.Lcd.print(WiFi.localIP());

  delay(1000);
  server.on("/", handle_root);

  server.begin();
  Serial.println("HTTP server started");
  delay(100);
  connectTovMix();
}

void loop()
{
  server.handleClient();
  btnM5.update();
  if (btnM5.isClick()) {
    if (screen == 0) {
      showNetworkScreen();
    } else if (screen == 1) {
      showTallyScreen();
    }
  }
  while (client.available())
  {
    String data = client.readStringUntil('\r\n');
    handleData(data);
  }
}

void cls()
{
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setCursor(0, 0);
}
