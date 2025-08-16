WiFiClient client;
WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)

PinButton btnM5(37);
PinButton btnAction(39);

PluginManager pm;

char currentState = -1;
char screen = 0;
bool connectedTovMix = false;

char deviceName[32];
int status = WL_IDLE_STATUS;
bool apEnabled = false;
boolean started = false;

// Time measure
int interval = 5000;
unsigned long lastCheck = 0;
unsigned long lastConnCheck = 0;
unsigned long lastBattCheck = 0;
unsigned long lastAccCheck = 0;
unsigned long sigStrengthChk = 0;
int screenRotation = 3;

float accX = 0;
float accY = 0;
float accZ = 0;

long rssi = -100;
String rssiLabel = "";

void setup()
{
  uint8_t c;
  setCpuFrequencyMhz(80); //Thanks Irvin Cee
  Serial.begin(115200);
  M5.begin();

  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  
  #if C_PLUS == 0 || C_PLUS == 1
    M5.IMU.Init();
  #else 
    M5.Imu.init();
  #endif
  
  delay(10);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  loadSettings();
  
  Serial.print("SSID: ");
  Serial.println(&(WIFI_SSID[0]));
  Serial.print("PASS: ");
  Serial.println(&(WIFI_PASS[0]));
}

void loop()
{
  server.handleClient();

  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n'); // e.g., WRITE index.html 123
    if (cmd.startsWith("WRITE ")) {
      int space1 = cmd.indexOf(' ', 6);
      String filename = cmd.substring(6, space1);
      int length = cmd.substring(space1 + 1).toInt();

      File file = SPIFFS.open("/" + filename, FILE_WRITE);
      if (!file) {
        Serial.println("FILE_ERROR");
        return;
      }

      int bytesRead = 0;
      while (bytesRead < length) {
        if (Serial.available()) {
          char c = Serial.read();
          file.write(c);
          bytesRead++;
        }
      }
      file.close();
      Serial.println("OK");
    }

    if (cmd.startsWith("DELETE ")) {
      Serial.print("GETTING IN THE DELTE IF");
      String filename = cmd.substring(7);
      filename.trim();
    
      if (!filename.startsWith("/")) filename = "/" + filename;
    
      Serial.print("Trying to delete file: ");
      Serial.println(filename);
    
      if (SPIFFS.exists(filename)) {
        bool success = SPIFFS.remove(filename);
        Serial.println(success ? "OK" : "REMOVE_FAIL");
      } else {
        Serial.println("NOT_FOUND");
      }
    }
  }
  
  btnM5.update();
  if (btnM5.isClick()) {
    if (screen == 0) {
      showNetworkScreen();
    } else if (screen == 1) {
      showTallyNum();
    } else if (screen == 2) {
      showBrightnessScreen();
    } else if (screen == 3) {
      showTallyScreen();
    }
  }

  #if C_PLUS == 0 || C_PLUS == 1
    M5.IMU.getAccelData(&accX, &accY, &accZ);
  #else
    M5.Imu.getAccelData(&accX, &accY, &accZ);
  #endif
  

  if(millis() > lastAccCheck + 1000){
    lastAccCheck = millis();
    if(accX > 0.70){
      if(screenRotation != 1){
        screenRotation = 1;
        M5.Lcd.setRotation(1);
        renderCurrentScreen();
      }
    } else if(accX < -0.70){
      if(screenRotation != 3){
        screenRotation = 3;
        M5.Lcd.setRotation(3);
        renderCurrentScreen();
      }
    }
    if(screen != 1 && screen != 4){
      if(accY > 0.70){
        if(screenRotation != 0){
          screenRotation = 0;
          M5.Lcd.setRotation(0);
          renderCurrentScreen();
        }
      } else if (accY < -0.70){
        if(screenRotation != 2){
          screenRotation = 2;
          M5.Lcd.setRotation(2);
          renderCurrentScreen();
        }
      }
    }
  }

  if(!started){
    started = true;
    start();
  }
  
  btnAction.update();
  if (btnAction.isClick() && screen == 0) {
    connectTovMix(false);
  }
  if(btnAction.isLongClick() && screen == 0){
    if(!client.connected()){
       resetSettings();
    } else {
      resetScreen();
      M5.Lcd.println("No need to reset");
      M5.Lcd.println("You're connected");
      delay(3000);
      showTallyScreen();
    }
  }

  if(btnAction.isClick() && screen == 3){
     updateBrightnessVar();
  }

  if(btnAction.isDoubleClick() && screen == 2){
    increaseTally();
    showTallyNum();
  }
  if(btnAction.isLongClick() && screen == 2){
    resetTally();
    showTallyNum();
  }
  
  while (client.available())
  {
    server.handleClient();
    String data = client.readStringUntil('\r\n');
    handleData(data);
  }

  if(millis() > lastBattCheck + 1000){
    if(screen == 0 && client.available()){
      lastBattCheck = millis();
      renderBatteryLevel(); 
    }
  }

  if(screen == 1 && millis() > sigStrengthChk + interval){
    rssi = WiFi.RSSI();
    sigStrengthChk = millis();
    Serial.println(rssi);
    if(rssi < 0 && rssi > -67) {
      rssiLabel = "strong";
    } else if(rssi < -67 && rssi > -85) {
      rssiLabel = "average";
    } else if (rssi < -85){
      rssiLabel = "poor";
    }
    showNetworkScreen();
  }

  if (CONN_INT != 0 && !client.connected() && !apEnabled && millis() > lastConnCheck + (CONN_INT * 1000))
  {
    client.stop();
    Serial.println("Reconnecting, based on given interval");
    singleReconnect();
  }
  
  if (!client.connected() && !apEnabled && millis() > lastCheck + interval)
  {
    client.stop();
    lastCheck = millis();
    Serial.println("Not connected anymore");
    noConnectionTovMix();
  }
}

void cls()
{
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setCursor(0, 0);
}

void start()
{
  cls();
  loadSettings();
  screenRotation = screenRotation == 1 || screenRotation == 3 ? screenRotation : 3;
  M5.Lcd.setRotation(screenRotation);
  
  String prod = "vMix M5Stick-C Tally";
  String author = "by Guido Visser";
  
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println("v"+semver);
  M5.Lcd.setCursor(lcdCoordX(20), lcdCoordY(20));
  M5.Lcd.println(prod);
  M5.Lcd.setCursor(lcdCoordX(35), lcdCoordY(40));
  M5.Lcd.println(author);
  
  delay(2000);

  startWiFi();
}

void resetScreen(){
  cls();
  digitalWrite(LED_BUILTIN, HIGH);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(WHITE, BLACK);
}

void renderBatteryLevel() {
  int battLvl = getBatteryLevel();
  if(battLvl > 100){
    battLvl = 100; 
  }
  if(C_PLUS){
    M5.Lcd.setCursor(10,10);
    M5.Lcd.setTextSize(2);
  } else {
    M5.Lcd.setCursor(0,0);
    M5.Lcd.setTextSize(1);
  }
  M5.Lcd.setCursor(0,0);
  M5.Lcd.print("Batt: ");
  M5.Lcd.print(battLvl);
  M5.Lcd.println("%");
}

int getBatteryLevel(void)
{
  #if C_PLUS != 2
    uint16_t vbatData = M5.Axp.GetVbatData();
    double vbat = vbatData * 1.1 / 1000;
    return floor(100.0 * ((vbat - 3.0) / (4.07 - 3.0)));
  #else
    return M5.Power.getBatteryLevel();
  #endif
}

void renderCurrentScreen(){
  if(!client.connected()){
    return;
  }
  if(screen == 0){
    showTallyScreen();
  } else if (screen == 1) {
    showNetworkScreen();
  } else if (screen == 2) {
    showTallyNum();
  } else if (screen == 4) {
    showAPScreen();
  }
}

int lcdCoordX(int x){
  if(C_PLUS){
    return x * 1.5;
  } else {
    return x;
  }
}
int lcdCoordY(int y){
  if(C_PLUS){
    return y * 1.6875;
  } else {
    return y;
  }
}
