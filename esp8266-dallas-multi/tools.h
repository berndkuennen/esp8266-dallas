
/*
 * get temp data all sensors and save data to central array
 * (cronned code)
 */
String get_all_sensor_data(){
  for(int i=0; i<numberOfDevices; i++){
    float celsius = dallas_sensors.getTempC(myDeviceData[i].devAddr);
    myDeviceData[i].celsius    =  celsius;
    myDeviceData[i].fahrenheit = (celsius * 9.0 / 5.0) + 32.0 ;
  }
}

String sensor_data_to_json(DeviceAddress deviceAddress){
  int deviceNum = NULL;
  String json   = "";

  for(uint8_t  i=0; i<numberOfDevices; i++){
    if( deviceAddress == myDeviceData[i].devAddr) { deviceNum = i; }
  }
  if (deviceNum != NULL){
    String json = "{\"c\": "   + String( myDeviceData[deviceNum].celsius )
                +  ", \"f\": " + String( myDeviceData[deviceNum].fahrenheit )
                +  "}";
  } else {
    json = "{}";
  }
  return json;
}

/*
 * convert byte to hex chars
 */
String byte2hex(int convertible)
{
  char hexdigit[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
  char result[3];
  result[0] = hexdigit[ convertible >> 4   ];
  result[1] = hexdigit[ convertible &  0xF ];
  result[2] = '\0';
  return result;
}


/*
 * build hex string "0x1234abcd" from device address
 */
void toHexString(DeviceAddress deviceAddress) {
  hx[0] = '0';
  hx[1] = 'x';
  for (uint8_t i = 0; i < 8; i++){
    hx[2+i*2] = byte2hex(deviceAddress[i])[0];
    hx[3+i*2] = byte2hex(deviceAddress[i])[1];
  }
}

/*
 * look for sensors and save their device addresses, plain and as hex string
 */
void find_sensor_addresses(){
  DeviceAddress tempDeviceAddress;
  char tempHexString[18];
  
  //-- check num of sensors; print error if there's too much
  numberOfDevicesFound = dallas_sensors.getDeviceCount();
  Serial.println("Found " + String(numberOfDevicesFound) + " sensors");
  if (numberOfDevicesFound>maxNumDevices){
    Serial.println("but can handle max. " + String(maxNumDevices) + " => omitting rest");
    numberOfDevices = maxNumDevices;
  }
  else  numberOfDevices = numberOfDevicesFound;

  //-- fill internal array with addresses in plain and as hex string
  for(int i=0;i<numberOfDevices; i++){
    // Search the wire for address
    if(dallas_sensors.getAddress(tempDeviceAddress, i)){
      memcpy( myDeviceData[i].devAddr, tempDeviceAddress, sizeof(tempDeviceAddress) );
      toHexString(tempDeviceAddress);
      memcpy( myDeviceData[i].hexName, hx, sizeof(myDeviceData[i].hexName) );
      Serial.print("Found device "   + String( i ));
      Serial.print(" with address: " + String( myDeviceData[i].hexName ) );
      Serial.println();
    } else {
      Serial.print("Found ghost device at " + String(i) );
      Serial.print(" but could not detect address. Check power and cabling");
      Serial.println();
    }
  }
}
