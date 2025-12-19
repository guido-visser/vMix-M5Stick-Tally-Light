WiFiClient client;
WebServer server(80);  // Object of WebServer(HTTP port, 80 is default)

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

// Prototypes (only what is contained in this file)
void drawWiFiIcon(int x, int y);
void renderBatteryLevel();
int getBatteryLevel(void);
void renderCurrentScreen();
int lcdCoordX(int x);
int lcdCoordY(int y);
void cls();
void start();
void resetScreen();

// Battery smoothing + redraw control
int battShown = -1;
unsigned long lastBattUiUpdate = 0;
char lastBattState = -1;
int lastBattDrawn = -1;

// -------------------- Battery colors --------------------
uint16_t getBatteryBgColor() {
  if (currentState == '1') return RED;    // LIVE
  if (currentState == '2') return GREEN;  // PRE
  return BLACK;                           // SAFE
}

uint16_t getBatteryTextColor() {
  if (currentState == '2') return BLACK;  // PRE -> zwart
  return WHITE;                           // LIVE & SAFE -> wit
}

// -------------------- Battery icon --------------------
void drawBatteryIcon(int x, int y, int pct, uint16_t fg, uint16_t bg) {
  pct = constrain(pct, 0, 100);

  int w = 28;
  int h = 14;

  // clear background (small area around)
  M5.Lcd.fillRect(x - 2, y - 2, w + 6, h + 4, bg);

  // outline + nippel
  M5.Lcd.drawRect(x, y, w, h, fg);
  M5.Lcd.fillRect(x + w, y + h / 4, 3, h / 2, fg);

  // inside empty
  M5.Lcd.fillRect(x + 2, y + 2, w - 4, h - 4, bg);

  // filling
  int fillW = map(pct, 0, 100, 0, w - 4);
  M5.Lcd.fillRect(x + 2, y + 2, fillW, h - 4, fg);
}

void setup()
{
  setCpuFrequencyMhz(80); //Thanks Irvin Cee
  Serial.begin(115200);
  M5.begin();

  #if C_PLUS == 2
    M5.Display.setBrightness(180); // failsafe at boot
    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);   // HOLD high: keeps the Plus2 running on battery power
  #endif

  #if C_PLUS == 0 || C_PLUS == 1
    M5.IMU.Init();
  #else
    M5.Imu.init();
  #endif

  delay(10);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  loadSettings();

  Serial.print("SSID: ");
  Serial.println(&(WIFI_SSID[0]));
  Serial.print("PASS: ");
  Serial.println(&(WIFI_PASS[0]));
}

void loop()
{
  server.handleClient();

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

  if (screen == 0 && millis() - lastBattCheck > 5000) {   
    lastBattCheck = millis();
    renderBatteryLevel();
    drawWiFiIcon(M5.Lcd.width() - 30, 2); 
}


  if(screen == 1 && millis() > sigStrengthChk + interval){
    rssi = WiFi.RSSI();
    sigStrengthChk = millis();

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
    singleReconnect();
  }

  if (!client.connected() && !apEnabled && millis() > lastCheck + interval)
  {
    client.stop();
    lastCheck = millis();
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

  screenRotation = (screenRotation == 1 || screenRotation == 3) ? screenRotation : 3;
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
  digitalWrite(LED_BUILTIN, LOW);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(WHITE, BLACK);
}

// Battery render (icon + big percent)
void renderBatteryLevel() {
  int battLvl = getBatteryLevel();
  battLvl = constrain(battLvl, 0, 100);

  if (battShown < 0) battShown = battLvl;

  int diff = battLvl - battShown;
  if (abs(diff) >= 3) {
    battShown += (diff > 0) ? 1 : -1;
  }

  // Detect state change BEFORE updating lastBattState
  bool stateChanged = (currentState != lastBattState);
  bool valueChanged = (battShown != lastBattDrawn);

  // Force redraw on tally state change
  if (stateChanged) {
    lastBattState = currentState;
    lastBattUiUpdate = 0;
  }

  // Throttle redraws, but NEVER block first draw / changes
  if (!stateChanged && !valueChanged && (millis() - lastBattUiUpdate < 2000)) {
    return;
  }
  lastBattUiUpdate = millis();

  // Remember what we drew
  lastBattDrawn = battShown;

  uint16_t bg = getBatteryBgColor();
  uint16_t fg = getBatteryTextColor();

  // clean the fixed area at the top left
  M5.Lcd.fillRect(0, 0, 140, 26, bg);

  // icon on the left
  drawBatteryIcon(4, 8, battShown, fg, bg);

  // text right
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(fg, bg);
  M5.Lcd.setCursor(42, 4);
  M5.Lcd.print(battShown);
  M5.Lcd.print("% ");

  // (optional) Wi-Fi icon in the top right corner
  // drawWiFiIcon(M5.Lcd.width() - 28, 2);
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

// WiFi icon (RSSI bars / AP / disconnected)
void drawWiFiIcon(int x, int y)
{
  uint16_t bg;
  if (currentState == '1') bg = RED;
  else if (currentState == '2') bg = GREEN;
  else bg = BLACK;

  uint16_t fg = (currentState == '2') ? BLACK : WHITE;

  int barW = 5;
  int gap  = 3;
  int h1 = 8, h2 = 12, h3 = 16;

  // Background clear
  M5.Lcd.fillRect(x - 2, y - 2, 3 * (barW + gap) + 8, h3 + 6, bg);

  // Not connected
  if (!client.connected() && !apEnabled) {
    M5.Lcd.drawLine(x, y, x + 18, y + 18, fg);
    M5.Lcd.drawLine(x + 18, y, x, y + 18, fg);
    return;
  }

  // AP mode
  if (apEnabled) {
    M5.Lcd.drawRect(x + 6, y + 2, 10, 14, fg);
    M5.Lcd.fillRect(x + 6, y + 18, 10, 3, fg);
    return;
  }

  int rssiNow = WiFi.RSSI();
  int bars = 1;
  if (rssiNow > -67) bars = 3;
  else if (rssiNow > -80) bars = 2;

  int baseY = y + h3;

  if (bars >= 1) M5.Lcd.fillRect(x + 0 * (barW + gap), baseY - h1, barW, h1, fg);
  if (bars >= 2) M5.Lcd.fillRect(x + 1 * (barW + gap), baseY - h2, barW, h2, fg);
  if (bars >= 3) M5.Lcd.fillRect(x + 2 * (barW + gap), baseY - h3, barW, h3, fg);
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