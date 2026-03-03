

String response_type_plain = "text/plain";
String response_type_json  = "application/json";

/*
 * send temperature data of all sensors as json
 */
void webhdl_send_sensor_all_json(){

  String json = "{\n  \"sensors\":\n  {\n";
  for(int i=0;i<numberOfDevices; i++){
    json += "    \""  +  String( myDeviceData[i].hexName ) + "\": ";  // add hex address as key
    json += sensor_data_to_json(myDeviceData[i].devAddr );               // add {c:... f:...} "data line" as value
    if (i < numberOfDevices-1) json += ",";
    json += "\n";
  }
  json += "  }\n}\n";
  
  server.send( 200, response_type_json, json);
  Serial.println("called webhdl_send_sensor_all_json()");
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
  Serial.println("called webhdl_send_sensor_addresses()");
}


/*
 * handler for individual sensor data
 * - send temp plain as °C, °F or both as JSON, depending on URI called
 */
void webhdl_send_sensor_data(){

  // which uri has been called?
  String uri_called = server.uri();
  Serial.print("uri called is ");   Serial.println(uri_called);

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
      response_text = sensor_data_to_json(myDeviceData[i].devAddr);
      String response_type = response_type_json;
    }
  }

  // send response
  server.send( 200, response_type, response_text);
  Serial.println("called webhdl_send_sensor_data()");
}

//-- web root
void webhdl_root() {
  server.send(200, response_type_plain, "esp8266-dallas-multi v1.0\n");
}

//-- page not found
void webhdl_http404(){
  server.send(404, response_type_plain, "404: Not found");
}

