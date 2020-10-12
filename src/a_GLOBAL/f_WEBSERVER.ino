// WEBSERVER STUFF
String HEADER = "<!DOCTYPE html>\
  <html lang='en'>\
  <head>\
  <meta charset='UTF-8'>\
  <meta name='viewport' content='width=device-width, initial-scale=1.0, shrink-to-fit=no'>\
  <title>vMix M5Stick-C Tally</title>\
  <style>\
  @import url(https://fonts.googleapis.com/css2?family=Open+Sans&display=swap);.wrapper,input[type=text],input[type=number],input[type=submit],select{width:100%;box-sizing:border-box}body,html{background:#2b2b2b;color:#eee;padding:0;margin:0;font-family:'Open Sans',verdana,sans-serif}.wrapper{padding:10px}.wrapper h1{text-align:center}input[type=text],input[type=number]{margin-bottom:10px}input,select{background-color:#6d6d6d;color:#f0f0f0;border:1px solid #000;font-size:18px;height:35px;padding:0 5px}input[type=submit]{height:50px;margin:10px auto}@media screen and (min-width:600px){.wrapper{width:600px;margin:0 auto}}\
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
    String bright = (String)BRIGHTNESS;
    String HTML = HEADER;
    HTML += "<div class=wrapper data-theme=light><h1>vMix M5Stack Tally Settings</h1><form action=/save id=frmData method=post onsubmit=return!1><div>SSID:<br><select id=ssid><option disabled selected>Scanning wifi...</select></div><div class=ssidCustomDiv style=display:none>Hidden SSID Name:<br><input id=ssidCustom type=text value='" + (String)WIFI_SSID + "'name=ssidCustom></div><div>Password:<br><input id=pwd type=text value='" + (String)WIFI_PASS + "'name=pwd></div><div>vMix IP Address:<br><input id=vmixip type=text value='" + (String)VMIX_IP + "'name=vmixip></div><div>Main Tally Number:<br><input id=tally_num type=number value='" + tally + "'name=tally_num max=1000 min=1></div><div>Multi Input (comma separated):<br><input id=m_tally type=text value='" + (String)M_TALLY + "'name=m_tally></div><div>Brightness:<br><select id=drpBright name=bright><option value=7>0%<option value=8>20%<option value=9>40%<option value=10>60%<option value=11>80%<option value=12>100%</select></div><input id=btnSave type=submit value=SAVE class='btn btn-primary'></form><h2>Reconnect to vMix</h2><form action=/reconnect id=frmReconnect method=post onsubmit=return!1><input id=btnReconnect type=submit value=RECONNECT></form></div><script>const btnSave = document.querySelector('#btnSave'); const btnReconnect = document.querySelector('#btnReconnect'); const drpBright = document.querySelector('#drpBright'); const ssidSelect = document.querySelector('#ssid'); drpBright.value = '" + bright + "'; btnSave.addEventListener('click', async function(e){ e.preventDefault(); let ssid = document.querySelector('#ssid').value; if(ssid === '__hidden__'){ ssid = document.querySelector('#ssidCustom').value; } const pwd = document.querySelector('#pwd').value; const vmixip = document.querySelector('#vmixip').value; const m_tally = document.querySelector('#m_tally').value; const frmData = document.querySelector('#frmData'); const tally_num = document.querySelector('#tally_num').value; let formData = new FormData(); formData.append('ssid', ssid.trim()); formData.append('pwd',pwd.trim()); formData.append('vmixip', vmixip.trim()); formData.append('m_tally', m_tally.trim().replace(/[^0-9,]+/g, '')); formData.append('tally_num', tally_num); btnSave.setAttribute('disabled', ''); const res = await fetch('/save', { method: 'POST', cache: 'no-cache', referrerPolicy: 'no-referrer', body: formData }); if(res.status === 200){ btnSave.value = 'SETTINGS SAVED!'; await setTimeout(()=>{btnSave.value = 'SAVE';}, 3000); } btnSave.removeAttribute('disabled'); }); btnReconnect.addEventListener('click', function(e){ e.preventDefault(); fetch('/reconnect'); }); ssidSelect.addEventListener('change', e => { const val = e.target.value; const ssidcd = document.querySelector('.ssidCustomDiv'); if(val === '__hidden__'){ ssidcd.style.display = 'block'; } else { ssidcd.style.display = 'none'; } }); document.addEventListener('DOMContentLoaded', async function(){ const res = await fetch('/scanNetwork'); res.text().then(text=>{ let networks = [text]; let str = ''; if(text.indexOf('|||') !== -1){ networks = text.split('|||'); } let sel = document.getElementById('ssid'); sel.innerHTML = ''; let existingNetwork = ''; networks.forEach(network => { let opt = document.createElement('option'); opt.appendChild( document.createTextNode(network) ); opt.value = network; if('" + (String)WIFI_SSID + "' === network){ existingNetwork = network; } sel.appendChild(opt); }); let opt = document.createElement('option'); opt.appendChild( document.createTextNode('Hidden network') ); opt.value = '__hidden__'; sel.appendChild(opt); if(existingNetwork !== ''){ sel.value = existingNetwork; } }); });</script>";
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
    server.send(200, "text/plain", message);
    Serial.println(message);

    String tally = server.arg("tally_num");
    String bright = server.arg("bright");

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

void startServer()
{
    server.on("/", handle_root);
    server.on("/save", handle_save);
    server.on("/reconnect", handleReconnect);
    server.on("/scanNetwork", handleScanNetwork);
    server.begin();
    Serial.println("HTTP server started");
}
