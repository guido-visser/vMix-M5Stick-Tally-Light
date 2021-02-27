// WEBSERVER STUFF
String HEADER = "<!DOCTYPE html>\
  <html lang='en'>\
  <head>\
  <meta charset='UTF-8'>\
  <meta name='viewport' content='width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no, shrink-to-fit=no'>\
  <title>vMix M5Stick-C Tally</title>\
  <style>\
  @import url(https://fonts.googleapis.com/css2?family=Open+Sans&display=swap);.wrapper,input[type=text],input[type=number],input[type=submit],select{width:100%;box-sizing:border-box}body,html{background:#2b2b2b;color:#eee;padding:0;margin:0;font-family:'Open Sans',verdana,sans-serif}.wrapper{padding:10px}.wrapper h1{text-align:center}input[type=text],input[type=number],select{margin-bottom:10px}input,select{background-color:#6d6d6d;color:#f0f0f0;border:1px solid #000;font-size:18px;height:35px;padding:0 5px}input[type=submit]{height:50px;margin:0 auto}@media screen and (min-width:600px){.wrapper{width:600px;margin:0 auto}}\
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
    String justLive = (String)JUSTLIVE;
    String bright = (String)BRIGHTNESS;
    String _mode = (String)MODE;
    String HTML = HEADER;
    
    HTML += "<div class=wrapper data-theme=light><h1>vMix M5Stack Tally Settings</h1><form action=/save id=frmData method=post onsubmit=return!1><div>SSID:<br><select id=ssid><option disabled selected>Scanning wifi...</option></select></div><div class=ssidCustomDiv style=display:none>Hidden SSID Name:<br><input id=ssidCustom type=text value='" + (String)WIFI_SSID + "'name=ssidCustom></div><div>Password:<br><input id=pwd type=text value='" + (String)WIFI_PASS + "'name=pwd></div><div>vMix IP Address:<br><input id=vmixip type=text value='" + (String)VMIX_IP + "'name=vmixip></div><div>Main Tally Number:<br><input id=tally_num type=number value='" + tally + "'name=tally_num max=1000 min=1></div><div>Multi Input (comma separated):<br><input id=m_tally type=text value='" + (String)M_TALLY + "'name=m_tally></div><div>Reconnect interval (in seconds, 0 means no reconnection interval):<br><input id=conn_int type=number value='" + CONN_INT + "'name=conn_int></div><div>Brightness:<br><select id=drpBright name=bright><option value=7>0%</option><option value=8>20%</option><option value=9>40%</option><option value=10>60%</option><option value=11>80%</option><option value=12>100%</option></select></div><div>Just Live:<br><select id=drpJustLive name=justLive><option value=0>False</option><option value=1>True</option></select></div><div>Mode:<br><select id=drpMode name=mode><option value=0>Text (SAFE, PRE, LIVE)</option><option value=1>Tally Number</option></select></div><input id=btnSave type=submit value=SAVE class='btn btn-primary'></form><h2>Reconnect to vMix</h2><form action=/reconnect id=frmReconnect method=post onsubmit=return!1><input id=btnReconnect type=submit value=RECONNECT></form><h2>Update firmware</h2><form action=/auth id=frmUpdate method=post onsubmit=return!1><input id=btnUpdate type=submit value=UPDATE></form></div><script>const btnSave = document.querySelector('#btnSave'); const btnReconnect = document.querySelector('#btnReconnect'); const btnUpdate = document.querySelector('#btnUpdate'); const drpBright = document.querySelector('#drpBright'); const ssidSelect = document.querySelector('#ssid'); const drpJustLive = document.querySelector('#drpJustLive'); const drpMode = document.querySelector('#drpMode'); drpBright.value = '" + bright + "'; drpJustLive.value = '" + justLive + "'; drpMode.value = '" + _mode + "'; btnSave.addEventListener('click', async function(e){ e.preventDefault(); let ssid = document.querySelector('#ssid').value; if(ssid === '__hidden__'){ ssid = document.querySelector('#ssidCustom').value; } const pwd = document.querySelector('#pwd').value; const vmixip = document.querySelector('#vmixip').value; const m_tally = document.querySelector('#m_tally').value; const frmData = document.querySelector('#frmData'); const tally_num = document.querySelector('#tally_num').value; const bright = drpBright.value; const mode = drpMode.value; const justLive = drpJustLive.value; const conn_int = parseInt(document.querySelector('#conn_int').value, 10) || 0; let formData = new FormData(); formData.append('ssid', ssid.trim()); formData.append('pwd',pwd.trim()); formData.append('vmixip', vmixip.trim()); formData.append('m_tally', m_tally.trim().replace(/[^0-9,]+/g, '')); formData.append('tally_num', tally_num); formData.append('conn_int', conn_int); formData.append('bright', bright); formData.append('mode', mode); formData.append('justLive', justLive); btnSave.setAttribute('disabled', ''); const res = await fetch('/save', { method: 'POST', cache: 'no-cache', referrerPolicy: 'no-referrer', body: formData }); if(res.status === 200){ btnSave.value = 'SETTINGS SAVED!'; await setTimeout(()=>{btnSave.value = 'SAVE';}, 3000); } btnSave.removeAttribute('disabled'); }); btnReconnect.addEventListener('click', function(e){ e.preventDefault(); fetch('/reconnect'); }); btnUpdate.addEventListener('click', function(e){ e.preventDefault(); const pass = prompt('What is the update password?'); exe(pass); }); async function exe(pass){ let formData = new FormData(); formData.append('pass', pass); const call = await fetch('/auth', { method: 'POST', cache: 'no-cache', referrerPolicy: 'no-referrer', body: formData }); const res = await call.json(); if(res.access === 'granted'){ window.location.href = '/update'; } } ssidSelect.addEventListener('change', e => { const val = e.target.value; const ssidcd = document.querySelector('.ssidCustomDiv'); if(val === '__hidden__'){ ssidcd.style.display = 'block'; } else { ssidcd.style.display = 'none'; } }); async function scanNetwork() { const res = await fetch('/scanNetwork'); res.text().then(text=>{ let networks = [text]; let str = ''; if(text.indexOf('|||') !== -1){ networks = text.split('|||'); } let sel = document.getElementById('ssid'); sel.innerHTML = ''; let existingNetwork = ''; networks.forEach(network => { let opt = document.createElement('option'); opt.appendChild( document.createTextNode(network) ); opt.value = network; if('" + (String)WIFI_SSID + "' === network){ existingNetwork = network; } sel.appendChild(opt); }); let opt = document.createElement('option'); opt.appendChild( document.createTextNode('Hidden network') ); opt.value = '__hidden__'; sel.appendChild(opt); if(existingNetwork !== ''){ sel.value = existingNetwork; } }); } document.addEventListener('DOMContentLoaded', function(){ console.log('U_PASS', '" + U_PASS + "'); if('" + U_PASS + "' === ''){ const newPass = prompt('Please specify your update password'); let fData = new FormData(); fData.append('pass', newPass); async function savePassword(){ const call = await fetch('/savePass', { method: 'POST', cache: 'no-cache', referrerPolicy: 'no-referrer', body: fData }); if(call.status === 200){ alert('Password set!'); } } savePassword(); } scanNetwork(); });</script>";
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

void handleAuthentication(){
  if(server.method() == HTTP_POST){
    Serial.print("pass: ");
    Serial.println(server.arg("pass"));
    Serial.print("U_PASS: ");
    Serial.println(U_PASS);
    if(server.arg("pass") != U_PASS){
      server.send ( 200, "application/json", "{\"access\": \"denied\"}"); 
    }
    server.on("/update", handleUpdatePage);
    /*handling uploading firmware file */
    server.on("/firmware", HTTP_POST, []() {
      server.sendHeader("Connection", "close");
      server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
      ESP.restart();
    }, []() {
      HTTPUpload& upload = server.upload();
      if (upload.status == UPLOAD_FILE_START) {
        Serial.printf("Update: %s\n", upload.filename.c_str());
        if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_WRITE) {
        /* flashing firmware to ESP*/
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) { //true to set the size to the current progress
          Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
          Update.printError(Serial);
        }
      }
    });
    server.send ( 200, "application/json", "{\"access\": \"granted\"}"); 
  } else {
    server.sendHeader("Location", String("/"), true);
    server.send ( 302, "text/plain", ""); 
  }
}

void handleSavePass(){
  if(server.method() == HTTP_GET){
    server.send(405);
  } else {
    preferences.begin("vMixTally", false);
    Serial.println("SAVING U_PASS");
    U_PASS = server.arg("pass");
    Serial.print("U_PASS");
    Serial.println(U_PASS);
    preferences.putString("u_pass", &(U_PASS[0]));
    preferences.end();
  }
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

char handleUpdatePage(){
  const char* serverIndex = "<html><head><style>@import url(https://fonts.googleapis.com/css2?family=Open+Sans&display=swap);body,html{background:#2d2d2d;color:#fff;font-family:'Open Sans'}a,a:active,a:visited{color:#fff}.prg-bar{display:none;width:100%;height:25px;background:#2d2d2d;margin-top:10px;box-shadow:0 0 3px #000 inset;border-radius:5px;box-sizing:border-box;padding:5px}#prg{display:none}.prg-bar-inner{background:#616161;width:0%;height:15px;border-radius:4px;box-sizing:border-box}.wrapper{width:400px;margin:20px auto;text-align:center;background:#3e3d3d;box-shadow:0 0 5px #000;padding:10px}</style></head><body><script src=https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js></script><div class=wrapper><h3>Update Firmware</h3><p>Download the latest release (.bin file)<br>from <a href=https://github.com/guido-visser/vMix-M5Stick-Tally-Light/releases>GitHub</a> and upload it below</p><form action=# enctype=multipart/form-data id=upload_form method=POST><input type=file name=update> <input type=submit value=Update></form><div class=prg-bar><div class=prg-bar-inner></div></div><div id=prg>0%</div></div><script>$('form').submit(function(t){t.preventDefault(),$('input[type=submit]').attr('disabled','disabled'),$('#prg,.prg-bar').show();var r=$('#upload_form')[0],e=new FormData(r);$.ajax({url:'/firmware',type:'POST',data:e,contentType:!1,processData:!1,xhr:function(){var t=new window.XMLHttpRequest;return t.upload.addEventListener('progress',function(t){if(t.lengthComputable){var r=t.loaded/t.total;$('#prg').html(Math.round(100*r)+'%'),$('.prg-bar-inner').css('width',Math.round(100*r)+'%')}},!1),t},success:function(t,r){$('.wrapper').text('Firmware updated successfully')},error:function(t,r,e){}})})</script></body></html>";
 server.send(200, "text/html", serverIndex);
}

void startServer()
{
    server.on("/", handle_root);
    server.on("/save", handle_save);
    server.on("/reconnect", handleReconnect);
    server.on("/scanNetwork", handleScanNetwork);
    server.on("/auth", handleAuthentication);
    if(U_PASS == ""){
      server.on("/savePass", handleSavePass);
    }
    server.begin();
    Serial.println("HTTP server started");
}
