
//-- some definitions for web pages
String response_type_css   = "text/css";
String response_type_html  = "text/html";
String response_type_plain = "text/plain";
String response_type_json  = "application/json";

//-- simple logging function, writes to Serial
void logHttpCall(int rtCode){
  Serial.println( String(millis()) + " " + server.client().remoteIP().toString() + " " + server.uri() );
}


//---- organisational stuff, root page, css, etc. ----------------

void webhdl_Root() {
  String html = "<html>\n<head>\n<title>Main page</title>\n<link rel='stylesheet' href='/styles.css'>\n</head>\n";
  html += "<body style='padding-top:58px;'><div class='container'><header id='lb' class='lb-fixed'>\n<input type='checkbox' class='lb-cb' id='lb-cb'/>\n<div class='lb-menu lb-menu-right lb-menu-material'>\n<ul class='lb-navigation'>\n";
  html += "<li class='lb-header'>MultiTempSensorBox</li>\n";
  html += "<li class='lb-item'><a href='/_ac'>WiFi configuration page</a></li>\n";
  html += "<li class='lb-item'><a href='/status'>status page</a></li>\n";
  html += "</ul></header></div>\n";

  html += "<br/><table align='center' border='0'>\n<tr><th>Sensor ID</th><th>Celsius</th></tr>\n";
  for(int i = 0; i < numberOfDevices; i++){
    html += "  <tr><td>" + String(myDeviceData[i].hexName) + "</td><td>" + String(myDeviceData[i].celsius) + "</td></tr>\n";
  }

  html += "</table></body>\n</html>\n";
  server.send(200, response_type_html, html);
  logHttpCall(200);
}

void webhdl_CSS(){
  String css = "html{font-family:Helvetica,Arial,sans-serif;font-size:16px;-ms-text-size-adjust:100%;-webkit-text-size-adjust:100%;-moz-osx-font-smoothing:grayscale;-webkit-font-smoothing:antialiased}body{margin:0;padding:0}.base-panel{margin:0 22px 0 22px}.base-panel * label :not(.bins){display:inline-block;width:3.0em;text-align:right}.base-panel * .slist{width:auto;font-size:0.9em;margin-left:10px;text-align:left}input{-moz-appearance:none;-webkit-appearance:none;font-size:0.9em;margin:8px 0 auto}.lap{visibility:collapse}.lap:target{visibility:visible}.lap:target .overlap{opacity:0.7;transition:0.3s}.lap:target .modal_button{opacity:1;transition:0.3s}.overlap{top:0;left:0;width:100%;height:100%;position:fixed;opacity:0;background:#000;z-index:1000}.modal_button{border-radius:13px;background:#660033;color:#ffffcc;padding:20px 30px;text-align:center;text-decoration:none;letter-spacing:1px;font-weight:bold;display:inline-block;top:40%;left:40%;width:20%;position:fixed;opacity:0;z-index:1001}table{border-collapse:collapse;border-spacing:0;border:1px solid #ddd;color:#444;background-color:#fff;margin-bottom:20px}table.info,table.info>tfoot,table.info>thead{width:100%;border-color:#5C9DED}table.info>thead{background-color:#5C9DED}table.info>thead>tr>th{color:#fff}td,th{padding:10px 22px}thead{background-color:#f3f3f3;border-bottom:1px solid #ddd}thead>tr>th{font-weight:400;text-align:left}tfoot{border-top:1px solid #ddd}tbody,tbody>tr:nth-child(odd){background-color:#fff}tbody>tr>td,tfoot>tr>td{font-weight:300;font-size:.88em}tbody>tr:nth-child(even){background-color:#f7f7f7}table.info tbody>tr:nth-child(even){background-color:#EFF5FD}.lb-fixed{width:100%;position:fixed;top:0;left:0;z-index:1000;box-shadow:0 1px 3px rgba(0, 0, 0, 0.12), 0 1px 2px rgba(0, 0, 0, 0.24)}.lb-burger span,.lb-burger span::before,.lb-burger span::after{display:block;height:2px;width:26px;transition:0.6s ease}.lb-cb:checked~.lb-menu li .lb-burger span{background-color:transparent}.lb-cb:checked~.lb-menu li .lb-burger span::before,.lb-cb:checked~.lb-menu li .lb-burger span::after{margin-top:0}.lb-header{display:flex;flex-direction:row;justify-content:space-between;align-items:center;height:58px}.lb-menu-right .lb-burger{margin-left:auto}.lb-brand{font-size:1.6em;padding:18px 3px 18px 24px}.lb-menu{min-height:58px;transition:0.6s ease;width:100%}.lb-navigation{display:flex;flex-direction:column;list-style:none;padding-left:0;margin:0}.lb-menu a,.lb-item a{text-decoration:none;color:inherit;cursor:pointer}.lb-item{height:58px}.lb-item a{padding:18px 24px 18px 24px;display:block}.lb-burger{padding:18px 24px 18px 24px;position:relative;cursor:pointer}.lb-burger span::before,.lb-burger span::after{content:'';position:absolute}.lb-burger span::before{margin-top:-8px}.lb-burger span::after{margin-top:8px}.lb-cb{display:none}.lb-cb:not(:checked)~.lb-menu{overflow:hidden;height:58px}.lb-cb:checked~.lb-menu{transition:height 0.6s ease;height:100vh;overflow:auto}.dropdown{position:relative;height:auto;min-height:58px}.dropdown:hover>ul{position:relative;display:block;min-width:100%}.dropdown>a::after{position:absolute;content:'';right:10px;top:25px;border-width:5px 5px 0;border-color:transparent;border-style:solid}.dropdown>ul{display:block;overflow-x:hidden;list-style:none;padding:0}.dropdown>ul .lb-item{min-width:100%;height:29px;padding:5px 10px 5px 40px}.dropdown>ul .lb-item a{min-height:29px;line-height:29px;padding:0}@media screen and (min-width:768px){.lb-navigation{flex-flow:row;justify-content:flex-end;}.lb-burger{display:none;}.lb-cb:not(:checked)~.lb-menu{overflow:visible;}.lb-cb:checked~.lb-menu{height:58px;}.lb-menu .lb-item{border-top:0;}.lb-menu-right .lb-header{margin-right:auto;}.dropdown{height:58px;}.dropdown:hover>ul{position:absolute;left:0;top:58px;padding:0;}.dropdown>ul{display:none;}.dropdown>ul .lb-item{padding:5px 10px;}.dropdown>ul .lb-item a{white-space:nowrap;}}.lb-cb:checked+.lb-menu .lb-burger-dblspin span::before{transform:rotate(225deg)}.lb-cb:checked+.lb-menu .lb-burger-dblspin span::after{transform:rotate(-225deg)}.lb-menu-material,.lb-menu-material .dropdown ul{background-color:#263238;color:#fff}.lb-menu-material .active,.lb-menu-material .lb-item:hover{background-color:#37474f}.lb-menu-material .lb-burger span,.lb-menu-material .lb-burger span::before,.lb-menu-material .lb-burger span::after{background-color:#fff}\n";
  server.send(200, response_type_css, css);
  logHttpCall(200);
}

void webhdl_Status() {
  String json = "{\"uptime_ms\":" + String(millis()) + ", \"signal\":" + String(WiFi.RSSI()) + "}";
  server.send(200, response_type_json, json);
  logHttpCall(200);
}

//-- page not found
void webhdl_http404(){
  server.send(404, response_type_plain, "404: Not found");
  logHttpCall(404);
}


//---- data section --------------------------------

/*
 * send temperature data of all sensors as json
 */
void webhdl_send_sensor_all_json(){

  String json = "{\n  \"sensors\":\n  {\n";
  for(int i=0;i<numberOfDevices; i++){
    json += "    \""  +  String( myDeviceData[i].hexName ) + "\": ";  // add hex address as key
    json += sensor_data_to_json( i );                                 // add {c:... f:...} "data line" as value
    if (i < numberOfDevices-1) json += ",";
    json += "\n";
  }
  json += "  }\n}\n";
  
  server.send( 200, response_type_json, json);
  logHttpCall(200);
}

/*
 * show device addresses of all sensors found as json array
 */
void webhdl_send_sensor_addresses(){
  String response = "{\n  \"sensors\": [" ;
  for(int i=0;i<numberOfDevices; i++){
    response += "\"";
    response += myDeviceData[i].hexName;
    response += "\"";
    if (i < numberOfDevices-1) response += ",";
  }
  response += "]\n}";
  server.send( 200, response_type_json, response);
  logHttpCall(200);
}


/*
 * handler for individual sensor data
 * - send temp plain as °C, °F or both as JSON, depending on URI called
 */
void webhdl_send_sensor_data(){

  // which uri has been called?
  String uri_called = server.uri();

  // default answer
  String response_text = "NULL";
  String response_type = response_type_plain;

  // check if temp is requested for any sensor in °C
  for(int i=0;i<numberOfDevices; i++){
    if ( uri_called == "/sensor/" + String(myDeviceData[i].hexName) + "/c" ){
      // match found
      response_text = String(myDeviceData[i].celsius);
    }
  }

  // check if temp is requested for any sensor in °F
  for(int i=0;i<numberOfDevices; i++){
    if ( uri_called == "/sensor/" + String(myDeviceData[i].hexName) + "/f" ){
      // match found
      response_text = String(myDeviceData[i].fahrenheit);
    }
  }

  // check if temp is requested for any sensor as json
  for(int i=0;i<numberOfDevices; i++){
    if ( uri_called == "/sensor/" + String(myDeviceData[i].hexName) + "/json" ){
      // match found
      response_text = sensor_data_to_json(i);
      String response_type = response_type_json;
    }
  }

  // send response
  server.send( 200, response_type, response_text);
  logHttpCall(200);
}


