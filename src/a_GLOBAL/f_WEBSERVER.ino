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
    HTML += "<input type='password' name='pwd' value='" + (String)WIFI_PASS + "'>";
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

    String tally = server.arg("tally_num");

    // save value in preferences
    preferences.begin("vMixTally", false);
    if (tally != "")
    {
        TALLY_NR = std::atoi(tally.c_str());
        preferences.putUInt("tally", TALLY_NR);
        Serial.println("PUT TALLY NR");
    }
    if (server.arg("ssid") != "")
    {
        WIFI_SSID = server.arg("ssid");
        WIFI_PASS = server.arg("pwd");
        preferences.putString("wifi_ssid", &(WIFI_SSID[0]));
        preferences.putString("wifi_pass", &(WIFI_PASS[0]));
        Serial.println("PUT WIFI_SSID & WIFI PASS");
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
    start();
}

void startServer()
{
    server.on("/", handle_root);
    server.on("/save", handle_save);
    server.begin();
    Serial.println("HTTP server started");
}
