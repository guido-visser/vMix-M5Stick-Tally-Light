bool stm = 0;
bool rec = 0;

// Connect to vMix instance
boolean connectTovMix(bool recursive)
{
  resetScreen();
  Serial.println("Connecting to vMix...");
  M5.Lcd.println("Connecting to vMix...");

  if (client.connect(&(VMIX_IP[0]), VMIX_PORT))
  {
    connectedTovMix = true;
    M5.Lcd.println("Connected to vMix!");

    // Subscribe to the tally events
    client.println("SUBSCRIBE TALLY");
    client.println("SUBSCRIBE ACTS");
    showTallyScreen();
    return true;
  }
  else
  {
    if(recursive){
      return false;
    }
    char tryCount = 0;
    cls();
    M5.Lcd.println("Could not connect to vMix");
    M5.Lcd.println("Retrying: 0/3");
    boolean retry = false;
    for (uint8_t i = 0; i < 3; i++)
    {
      Serial.print(i);
      retry = retryConnectionvMix(i);
      if (!retry) {
        return true;
      }
    }
    cls();
    connectedTovMix = false;
    noConnectionTovMix();
    return false;
  }
}

void singleReconnect(){
  resetScreen();
  M5.Lcd.println("Connecting to vMix...");
  if (client.connect(&(VMIX_IP[0]), VMIX_PORT))
  {
    lastConnCheck = millis();
    connectedTovMix = true;
    M5.Lcd.println("Connected to vMix!");

    // Subscribe to the tally events
    client.println("SUBSCRIBE TALLY");
    showTallyScreen();
  } else {
    lastConnCheck = millis();
  }
}

boolean retryConnectionvMix(int tryCount) {
  cls();
  M5.Lcd.setTextSize(1);
  M5.Lcd.println("Couldn't connect to vMix");
  M5.Lcd.print("Retrying: ");
  M5.Lcd.print(tryCount);
  M5.Lcd.print("/3");
  delay(2000);
  boolean conn = connectTovMix(true);
  if (conn) {
    return false;
  }
  return true;
}

void posTallyNums()
{
  String s = String(TALLY_NR);

  // Ensure that textSize is already set before using this function!
  // (this happens in showTallyScreen)

  int w = M5.Lcd.textWidth(s);

  int x = (M5.Lcd.width() - w) / 2;
  int y;

  // Vertical position depending on rotation
  if (screenRotation == 1 || screenRotation == 3) {
    y = (M5.Lcd.height() / 2) - 20;   // landscape
  } else {
    y = (M5.Lcd.height() / 2) - 20;   // portrait (same offset works here too)
  }

  M5.Lcd.setCursor(x, y);
}

void setTallyProgram()
{
  digitalWrite(LED_BUILTIN, HIGH);
  M5.Lcd.fillScreen(RED);
  M5.Lcd.setTextColor(WHITE, RED);
  if(screenRotation == 1 || screenRotation == 3){
    M5.Lcd.setCursor(lcdCoordX(25), lcdCoordY(23));
    if(MODE == 0){
      M5.Lcd.println("LIVE");
    }
  } else if(screenRotation == 0 || screenRotation == 2) {
    M5.Lcd.setCursor(lcdCoordX(30), lcdCoordY(70));
    if(MODE == 0){
      M5.Lcd.println("L");
    }
  }
  if(MODE == 1){
    posTallyNums();
    M5.Lcd.println(TALLY_NR);
  }
  pm.onLive();
}

void setTallyPreview() {
  digitalWrite(LED_BUILTIN, LOW);
  M5.Lcd.fillScreen(GREEN);
  M5.Lcd.setTextColor(BLACK, GREEN);
  if(screenRotation == 1 || screenRotation == 3){
    M5.Lcd.setCursor(lcdCoordX(40), lcdCoordY(23));
    if(!JUSTLIVE && MODE == 0){
      M5.Lcd.println("PRE");
    }
  } else if(screenRotation == 0 || screenRotation == 2) {
    M5.Lcd.setCursor(lcdCoordX(30), lcdCoordY(70));
    if(!JUSTLIVE && MODE == 0){
      M5.Lcd.println("P");
    }
  }
  if(MODE == 1){
    posTallyNums();
    M5.Lcd.println(TALLY_NR);
  }
  pm.onPre();
}

void setTallyOff() {
  digitalWrite(LED_BUILTIN, LOW);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE, BLACK);
  if(screenRotation == 1 || screenRotation == 3){
    M5.Lcd.setCursor(lcdCoordX(23), lcdCoordY(23));
    if(!JUSTLIVE && MODE == 0){
      M5.Lcd.println("SAFE");
    }
  } else if(screenRotation == 0 || screenRotation == 2) {
    M5.Lcd.setCursor(lcdCoordX(30), lcdCoordY(70));
    if(!JUSTLIVE && MODE == 0){
      M5.Lcd.println("S");
    }
  }
  if(MODE == 1){
    posTallyNums();
    M5.Lcd.println(TALLY_NR);
  }
  pm.onSafe();
}

// Handle incoming data
void handleData(String data)
{
  bool changed = false;
  // Check if server data is tally data
  if (data.indexOf("TALLY") == 0)
  {
    char newState = data.charAt(TALLY_NR + 8);
    // Check if tally state has changed
    if (currentState != newState || screen == 1)
    {
      currentState = newState;
      changed = true;
      if(M_TALLY == ""){
        showTallyScreen();
      }
    }
    if(M_TALLY != ""){
      if(currentState != '1' && M_TALLY != ""){
        int str_len = M_TALLY.length() + 1;
       
        // Prepare the character array (the buffer) 
        char str[str_len];
         
        // Copy it over 
        M_TALLY.toCharArray(str, str_len);
        
        char * pch;
        pch = strtok(str, ",");
        while (pch != NULL)
        {
          String c(pch);
          char newMState = data.charAt(c.toInt() + 8);
          if((currentState == '0' && (newMState == '1' || newMState == '2')) || (currentState == '2' && (newMState == '1'))){
            currentState = newMState;
            changed = true;
          }
          pch = strtok(NULL, ",");
        }
      }
      if(changed){
        showTallyScreen();
      }
    }
  }
  else
  {
    if(data.indexOf("ACTS OK Recording 1") == 0){
      rec = 1;
    } else if(data.indexOf("ACTS OK Recording 0") == 0){
      rec = 0;
    } else if(data.indexOf("ACTS OK Streaming 1") == 0) {
      stm = 1;
    } else if(data.indexOf("ACTS OK Streaming 0") == 0){
      stm = 0;
    } else {
      Serial.print("Response from vMix: ");
      Serial.println(data);
    }
    showStatus();
  }
}

void showTallyScreen() {
  cls();
  screen = 0;
  if(C_PLUS){
    M5.Lcd.setTextSize(8);
  } else {
    M5.Lcd.setTextSize(5);
  }
  if(!JUSTLIVE){
    switch (currentState)
    {
      case '0':
        setTallyOff();
        break;
      case '1':
        setTallyProgram();
        break;
      case '2':
        setTallyPreview();
        break;
      default:
        setTallyOff();
    }
  } else {
    switch (currentState)
    {
      case '1':
        setTallyProgram();
        break;
      default:
        setTallyOff();
    }
  }
  
  renderBatteryLevel();
  showStatus();
  drawWiFiIcon(M5.Lcd.width() - 30, 2);

  // WiFi icon as the last thing to draw, so that REC/STM does not overwrite it
  int xWifi = M5.Lcd.width() - 30;
  int yWifi = 2;
  drawWiFiIcon(xWifi, yWifi);
}

void showStatus(){

  // --- set background/text color based on tally state ---
  uint16_t bg = (currentState == '1') ? RED : (currentState == '2') ? GREEN : BLACK;
  uint16_t fg = (currentState == '2') ? BLACK : WHITE;  // PRE -> black, otherwise white

  // --- text size ---
  if(C_PLUS){
    M5.Lcd.setTextSize(2);
  } else {
    M5.Lcd.setTextSize(1);
  }

  // Landscape (rotation 1/3): top right
  if(screenRotation == 1 || screenRotation == 3){
    // wide enough to cover both REC and STM
    int y = 0;
    int h = (C_PLUS ? 20 : 10) + 6;
    int w = (C_PLUS ? 90 : 70);
    int x = M5.Lcd.width() - w;
    M5.Lcd.fillRect(x, y, w, h, bg);
  } 
  // Portrait (rotation 0/2): bottom (where you print REC/STM)
  else {
    int h = (C_PLUS ? 20 : 10) + 6;
    int y = M5.Lcd.height() - h;
    M5.Lcd.fillRect(0, y, M5.Lcd.width(), h, bg);
  }

  M5.Lcd.setTextColor(fg, bg);

  // REC
  if(screenRotation == 1 || screenRotation == 3){
    if(C_PLUS){
      M5.Lcd.setCursor(200,0);
    } else {
      M5.Lcd.setCursor(140,0);
    }
  } else {
    if(C_PLUS){
      M5.Lcd.setCursor(95,220); 
    } else {
      M5.Lcd.setCursor(60,150); 
    }
  }
  if(rec){
    M5.Lcd.print("REC");
  } else {
    M5.Lcd.print("   ");
  }

  // STM
  if(screenRotation == 1 || screenRotation == 3){
    if(C_PLUS){
      M5.Lcd.setCursor(150,0);
    } else {
      M5.Lcd.setCursor(110,0); 
    }
  } else {
    if(C_PLUS){
      M5.Lcd.setCursor(10,220); 
    } else {
      M5.Lcd.setCursor(0,150); 
    }
  }
  if(stm){
    M5.Lcd.print("STM");
  } else {
    M5.Lcd.print("   ");
  }
}

void noConnectionTovMix(){
  resetScreen();
  M5.Lcd.println("Couldn't connect to vMix");
  M5.Lcd.println();
  M5.Lcd.println("vMix is closed");
  M5.Lcd.println("or check settings");
  M5.Lcd.println();
  M5.Lcd.print("http://");
  M5.Lcd.println(WiFi.localIP());
  M5.Lcd.println();
  M5.Lcd.println("Press BTN39 to reconnect");
}
