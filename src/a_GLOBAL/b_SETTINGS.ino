void loadSettings()
{
  Serial.println("LOAD SETTINGS");
  preferences.begin("vMixTally", false);
  if (preferences.getString("wifi_ssid").length() > 0)
  {
    Serial.println("LOADING FROM PREFERENCES, OVERRIDING HARDCODED VARIABLES");
    WIFI_SSID = preferences.getString("wifi_ssid");
    WIFI_PASS = preferences.getString("wifi_pass");
  }
  else
  {
      Serial.println("Use hardcoded settings here");
      saveWiFiPreferences(&(WIFI_SSID[0]), &(WIFI_PASS[0]));
  }
  
  if(preferences.getString("vmix_ip").length() > 0){
    Serial.println("SETTING TALLY_NR AND VMIX_IP");
    TALLY_NR = preferences.getUInt("tally");
    VMIX_IP = preferences.getString("vmix_ip");
    if(TALLY_NR > 9){
      tnlen = 2;
    }
  }

  Serial.println("SET CONN_INT");
  CONN_INT = preferences.getUInt("conn_int") || CONN_INT;
  Serial.println("SET MODE");
  MODE = preferences.getUInt("mode") || MODE;
  Serial.println("SET JUSTLIVE");
  JUSTLIVE = preferences.getUInt("justLive") || JUSTLIVE;

  if(preferences.getString("m_tally").length() > 0){
    Serial.println("SET M_TALLY");
    M_TALLY = preferences.getString("m_tally");
  }

  Serial.println("SET BRIGHTNESS");
  Serial.println(preferences.getUInt("bright"));

  if(preferences.getUInt("bright")){
    BRIGHTNESS = preferences.getUInt("bright");
    #if C_PLUS == 0 || C_PLUS == 1
      M5.Axp.ScreenBreath((BRIGHTNESS-6)*16);
    #endif
  }
  Serial.println("PREF END");
  preferences.end();
}

void saveWiFiPreferences(String wifi_ssid, String wifi_pass)
{
    Serial.println("PREF START");
    preferences.begin("vMixTally", false);
    if (wifi_ssid != "")
    {
      Serial.println("PUT WIFI_SSID " + wifi_ssid);
      preferences.putString("wifi_ssid", wifi_ssid);
      Serial.println("PUT WIFI_PASS " + wifi_pass);
      preferences.putString("wifi_pass", wifi_pass);
    }
    preferences.end();
    Serial.println("SET WIFI_SSID");
    WIFI_SSID = wifi_ssid;
    Serial.println("SET WIFI_PASS");
    WIFI_PASS = wifi_pass;
}

void resetSettings(){
  preferences.begin("vMixTally", false);
  
  preferences.putString("wifi_ssid", "");
  preferences.putString("wifi_pass", "");
  preferences.putString("vmix_ip", "");
  preferences.putUInt("tally", 1);
  preferences.putUInt("bright", 12);
  preferences.putString("m_tally", "");
  
  preferences.end();

  Serial.println("Settings are reset");
  resetScreen();
  M5.Lcd.println("Settings are reset");
  M5.Lcd.println();
  M5.Lcd.println("Please reboot device");
}

void increaseTally(){
  TALLY_NR++;
  preferences.begin("vMixTally", false);
  preferences.putUInt("tally", TALLY_NR);
  preferences.end();
}
void resetTally(){
  TALLY_NR = 1;
  preferences.begin("vMixTally", false);
  preferences.putUInt("tally", TALLY_NR);
  preferences.end();
}

void saveBrightness(){
  preferences.begin("vMixTally", false);
  preferences.putUInt("bright", BRIGHTNESS);
  preferences.end();
}

// Print settings
void printSettings()
{
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(&(WIFI_SSID[0]));
  Serial.print("SSID Password: ");
  Serial.println(&(WIFI_PASS[0]));
  Serial.print("vMix hostname: ");
  Serial.println(VMIX_IP);
  Serial.print("Tally number: ");
  Serial.println(TALLY_NR);
}
