void loadSettings()
{

}

void saveSettings()
{

}

void saveWiFiPreferences(String wifi_ssid, String wifi_pass)
{
    preferences.begin("vMixTally", false);
    if (wifi_ssid != "")
    {
        preferences.putString("wifi_ssid", wifi_ssid);
        preferences.putString("wifi_pass", wifi_pass);
    }
    preferences.end();
    WIFI_SSID = wifi_ssid;
    WIFI_PASS = wifi_pass;
}

void loadWiFiPreferences(){
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
  preferences.end();
}

void loadvMixPreferences()
{
    preferences.begin("vMixTally", false);
    
    if(preferences.getString("vmix_ip").length() > 0){
      Serial.println("");
      TALLY_NR = preferences.getUInt("tally");
      VMIX_IP = preferences.getString("vmix_ip");
    } else {
      
    }
      
    
    Serial.println(TALLY_NR);
    Serial.println(VMIX_IP);
    
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
