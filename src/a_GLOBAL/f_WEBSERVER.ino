void listSPIFFSFiles() {
  Serial.println("Listing SPIFFS files:");
  File root = SPIFFS.open("/");
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    Serial.print("FILE: ");
    Serial.print(file.name());
    Serial.print("\tSIZE: ");
    Serial.println(file.size());
    file = root.openNextFile();
  }
}

void handle_root()
{
  listSPIFFSFiles();
  Serial.println("HANDLE ROOT");
  File file = SPIFFS.open("/index.html", "r");
  if (!file) {
    Serial.print("FILE NOT FOUND");
    server.send(500, "text/plain", "File not found");
    return;
  }

  Serial.println("STREAMING FILE");
  server.streamFile(file, "text/html");
  Serial.println("END STREAMING FILE");
  file.close();
}

void handle_save()
{
    String message = "";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++)
    {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    message += " direct:" + server.arg("tally_num");
    server.send(200, "text/plain", message);
    Serial.println(message);

    String tally = server.arg("tally_num");
    String bright = server.arg("bright");
    String justLive = server.arg("justLive");
    String _mode = server.arg("mode");
    String conn_int = server.arg("conn_int");

    Serial.print("BRIGHTNESS: ");
    Serial.println(bright);

    // save value in preferences
    preferences.begin("vMixTally", false);
    if (tally != "")
    {
        TALLY_NR = std::atoi(tally.c_str());
        preferences.putUInt("tally", TALLY_NR);
        Serial.println("PUT TALLY NR");
    }
    if(bright != ""){
      BRIGHTNESS = std::atoi(bright.c_str());
      preferences.putUInt("bright", BRIGHTNESS);
      Serial.println("PUT BRIGHT");
    }
    if(justLive != ""){
      JUSTLIVE = std::atoi(justLive.c_str());
      preferences.putUInt("justLive", JUSTLIVE);
      Serial.println("PUT JUSTLIVE");
    }
    if(_mode != ""){
      MODE = std::atoi(_mode.c_str());
      preferences.putUInt("mode", MODE);
      Serial.println("PUT MODE");
    }
    
    if (server.arg("ssid") != "")
    {
        WIFI_SSID = server.arg("ssid");
        WIFI_PASS = server.arg("pwd");
        preferences.putString("wifi_ssid", &(WIFI_SSID[0]));
        preferences.putString("wifi_pass", &(WIFI_PASS[0]));
        Serial.println("PUT WIFI_SSID & WIFI PASS");
    }

    M_TALLY = server.arg("m_tally");
    preferences.putString("m_tally", &(M_TALLY[0]));

    if (conn_int != "")
    {
        CONN_INT = std::atoi(conn_int.c_str());
        preferences.putUInt("conn_int", CONN_INT);
        Serial.println("PUT CONN INT");
    }
    
    if (server.arg("vmixip") != "")
    {
        VMIX_IP = server.arg("vmixip");
        preferences.putString("vmix_ip", &(VMIX_IP[0]));
        Serial.println("PUT VMIX IP");
    }
    preferences.end();
    
    cls();

    //Reboot stick
    ESP.restart(); //Thanks to Babbit on vMix forums!
}

void handleReconnect(){
  connectTovMix(false);
  server.send(200, "text/plain", "success");
}

void handleScanNetwork(){
  int numSsid = WiFi.scanNetworks();
  if(numSsid == -1){
    
  } else {
    String retStr = "";
    for (int thisNet = 0; thisNet<numSsid; thisNet++) {
      if(thisNet == 0){
        retStr += WiFi.SSID(thisNet);
      } else {
        retStr += "|||" + WiFi.SSID(thisNet);
      }
    }
    server.send(200, "text/plain", retStr);
  }
}

void handleStickData() {
// Create a JSON document
  StaticJsonDocument<256> data;

  data["ssid"] = WIFI_SSID;
  data["pass"] = WIFI_PASS;
  data["vmix_ip"] = VMIX_IP;
  data["tally_num"] = TALLY_NR;
  data["m_tally"] = M_TALLY;
  data["conn_int"] = CONN_INT;
  data["bright"] = BRIGHTNESS;
  data["just_live"] = JUSTLIVE;
  data["mode"] = MODE;

  // Serialize JSON to string
  String json;
  serializeJson(data, json);

  server.send(200, "application/json", json);
}

void startServer()
{
    server.on("/", HTTP_GET, handle_root);
    server.on("/save", HTTP_POST, handle_save);
    server.on("/reconnect", HTTP_GET, handleReconnect);
    server.on("/scanNetwork", HTTP_GET, handleScanNetwork);
    server.on("/data", HTTP_GET, handleStickData);
    server.begin();
    Serial.println("HTTP server started");
}
