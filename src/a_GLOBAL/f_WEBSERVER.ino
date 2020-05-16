// WEBSERVER STUFF
String HTML = "<!DOCTYPE html>\
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
  <div class='wrapper'>\
    <h1>vMix M5Stick-C Tally</h1>\
    <form action='/save' method='post' enctype='multipart/form-data' data-ajax='false'>\
      Tally Number:<br/>\
      <input type='number' value='1' />\
      <input type='submit' value='SAVE' class='btn btn-primary'>\
    </form>\
  </div>\
</body>\
</html>";

void handle_root() {
  server.send(200, "text/html", HTML);
}
