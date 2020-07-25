// WEBSERVER STUFF
String HEADER = "<!DOCTYPE html>\
  <html lang='en'>\
  <head>\
  <meta charset='UTF-8'>\
  <meta name='viewport' content='width=device-width, initial-scale=1.0, shrink-to-fit=no'>\
  <title>vMix M5Stick-C Tally</title>\
  <link rel='stylesheet' type='text/css' href='style.css'>\
  <style>\
  .wrapper,input{width:100%}body,html{padding:0;margin:0}.wrapper{padding:10px;box-sizing:border-box}.wrapper h1{text-align:center}input[type=submit]{width:50px;margin:10px auto}\
  </style>\
  </head>\
  <body>\
  <div class='wrapper'>";

String FOOTER = "</div>\
  </body>\
  </html>";

void handle_root()
{
    String tally = (String)TALLY_NR;
    String HTML = HEADER;
    HTML += "<div class='wrapper'>";
    HTML += "<h1>vMix M5Stack Tally Settings</h1>";
    HTML += "<form action='/save' method='post'>";
    HTML += "SSID:<br/>";
    HTML += "<input type='text' name='ssid' value='" + (String)WIFI_SSID + "'>";
    HTML += "Password:<br/>";
    HTML += "<input type='text' name='pwd' value='" + (String)WIFI_PASS + "'>";
    HTML += "vMix IP Address:<br/>";
    HTML += "<input type='text' name='vmixip' value ='" + (String)VMIX_IP + "'>";
    HTML += "Tally Number:<br/>";
    HTML += "<input type='number' name='tally_num' value='" + tally + "'>";
    HTML += "<input type='submit' value='SAVE' class='btn btn-primary'>";
    HTML += "</form>";
    HTML += FOOTER;

    server.send(200, "text/html", HTML);
}

void handle_vmix_settings()
{
    String tally = (String)TALLY_NR;
    String HTML = HEADER;
    HTML += "<div class='wrapper'>";
    HTML += "<h1>vMix M5Stack Tally Settings</h1>";
    HTML += "<form action='/save_vmix_settings' method='post'>";
    HTML += "vMix IP Address:<br/>";
    HTML += "<input type='text' name='vmixip' value ='" + (String)VMIX_IP + "'>";
    HTML += "Tally Number:<br/>";
    HTML += "<input type='number' name='tally_num' value='" + tally + "'>";
    HTML += "<input type='submit' value='SAVE' class='btn btn-primary'>";
    HTML += "</form>";
    HTML += FOOTER;

    server.send(200, "text/html", HTML);
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
    server.send(404, "text/plain", message);
    Serial.println(message);

    showTallyNum(server.arg("tally_num"));
    String tally = server.arg("tally_num");

    // save value in preferences
    preferences.begin("vMixTally", false);
    if (tally != "")
    {
        TALLY_NR = std::atoi(tally.c_str());
        preferences.putUInt("tally", TALLY_NR);
    }
    if (server.arg("ssid") != "")
    {
        WIFI_SSID = server.arg("ssid");
        WIFI_PASS = server.arg("pwd");
        preferences.putString("wifi_ssid", &(WIFI_SSID[0]));
        preferences.putString("wifi_pass", &(WIFI_PASS[0]));
    }
    if (server.arg("vmixip") != "")
    {
        VMIX_IP = server.arg("vmixip");
        preferences.putString("vmix_ip", &(VMIX_IP[0]));
    }
    preferences.end();

    // We start by connecting to a WiFi network
    M5.Lcd.setTextSize(2);
    M5.Lcd.println(WIFI_SSID);
    M5.Lcd.println(WIFI_PASS);
    delay(2000);

    // WiFi.begin(&(WIFI_SSID[0]), &(WIFI_PASS[0]));

    M5.Lcd.println();
    M5.Lcd.setCursor(25, 60);
    M5.Lcd.println("Waiting for WiFi...");

    int tries = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        M5.Lcd.print(".");
        delay(1000);
        tries++;
        if (tries > 10)
        {
            tries = 0;
            break;
        }
    }
    cls();
    M5.Lcd.println("WiFi connected");
    M5.Lcd.println("IP address: ");
    M5.Lcd.print(WiFi.localIP());

    delay(2000);
    connectTovMix();
    //server.send(200, "text/html", RESP);
}

void handle_save_vmix_settings()
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
    server.send(404, "text/plain", message);
    Serial.println(message);

    showTallyNum(server.arg("tally_num"));
    String tally = server.arg("tally_num");

    // save value in preferences
    preferences.begin("vMixTally", false);
    if (tally != "")
    {
        TALLY_NR = std::atoi(tally.c_str());
        preferences.putUInt("tally", TALLY_NR);
    }
    if (server.arg("vmixip") != "")
    {
        VMIX_IP = server.arg("vmixip");
        preferences.putString("vmix_ip", &(VMIX_IP[0]));
    }
    preferences.end();
    startWiFi();
}

void startServer()
{
    server.on("/", handle_root);
    server.on("/save", handle_save);
    server.on("/vmix_settings", handle_vmix_settings);
    server.on("/save_vmix_settings", handle_save_vmix_settings);
    server.begin();
    //serverap.begin();
    Serial.println("HTTP server started");
    Serial.println("Connect to WiFi");
    //Serial.println((String)wifi_connected);
    delay(100);
}
