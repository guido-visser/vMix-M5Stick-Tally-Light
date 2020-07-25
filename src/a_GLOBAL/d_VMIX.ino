// Connect to vMix instance
boolean connectTovMix()
{
  cls();
  M5.Lcd.println("Connecting to vMix...");

  if (client.connect(&(VMIX_IP[0]), VMIX_PORT))
  {
    M5.Lcd.println("Connected to vMix!");

    // Subscribe to the tally events
    client.println("SUBSCRIBE TALLY");
    return true;
  }
  else
  {
    char tryCount = 0;
    cls();
    M5.Lcd.println("Could not connect to vMix");
    M5.Lcd.println("Retrying: 0/3");
    boolean retry = false;
    for (int i = 0; i < 3; ++i)
    {
      Serial.print(i);
      retry = retryConnectionvMix(i);
      if (!retry) {
        return true;
      }
    }
    cls();
    M5.Lcd.println("Couldn't connect to vMix");
    M5.Lcd.println();
    M5.Lcd.println("Please restart device");
    M5.Lcd.println("to retry");
    return false;
  }
}

boolean retryConnectionvMix(char tryCount) {
  cls();
  M5.Lcd.println("Couldn't connect to vMix");
  M5.Lcd.print("Retrying: ");
  M5.Lcd.print(tryCount);
  M5.Lcd.print("/3");
  delay(2000);
  boolean conn = connectTovMix();
  if (conn) {
    return false;
  }
  return true;
}

void setTallyProgram()
{
  digitalWrite(LED_BUILTIN, LOW);
  M5.Lcd.fillScreen(RED);
  M5.Lcd.setTextColor(WHITE, RED);
  M5.Lcd.setCursor(25, 23);
  M5.Lcd.println("LIVE");
}

void setTallyPreview() {
  digitalWrite(LED_BUILTIN, HIGH);
  M5.Lcd.fillScreen(GREEN);
  M5.Lcd.setTextColor(BLACK, GREEN);
  M5.Lcd.setCursor(40, 23);
  M5.Lcd.println("PRE");
}

void setTallyOff() {
  digitalWrite(LED_BUILTIN, HIGH);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setCursor(23, 23);
  M5.Lcd.println("SAFE");
}

// Handle incoming data
void handleData(String data)
{
  // Check if server data is tally data
  if (data.indexOf("TALLY") == 0)
  {

    char newState = data.charAt(TALLY_NR + 8);
    // Check if tally state has changed
    if (currentState != newState || screen == 1)
    {
      currentState = newState;
      showTallyScreen();
    }
  }
  else
  {
    Serial.print("Response from vMix: ");
    Serial.println(data);
  }
}

void showTallyScreen() {
  cls();
  screen = 0;
  M5.Lcd.setTextSize(5);
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
}
