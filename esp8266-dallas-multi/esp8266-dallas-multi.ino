/*
 * 
 * Copyright 2019.2026 Bernd Künnen
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of 
 * this software and associated documentation files (the "Software"), to deal in the 
 * Software without restriction, including without limitation the rights to use, copy, 
 * modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
 * and to permit persons to whom the Software is furnished to do so, subject to the 
 * following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all 
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * --------
 *
 * !!! IMPORTANT !!!
 * 
 * (1) If compiling with Arduino v1.8.9 (Snap) on Ubuntu Jammy 22.04 and your board
 * is an ESP8266, then you have to install max. v3.0.2 of its board library due
 * to python version mismatches.
 * See: https://forum.arduino.cc/t/arduino-2-0-rc-3-nodemcu-compile-cant-find-python3/946670/4
 *
 * --------
 * This piece of code was inspired by & thanks to the authors of:
 *  - https://randomnerdtutorials.com/esp8266-ds18b20-temperature-sensor-web-server-with-arduino-ide/
 *  - https://lastminuteengineers.com/multiple-ds18b20-esp8266-nodemcu-tutorial/
 *  - https://lastminuteengineers.com/multiple-ds18b20-arduino-tutorial/
 *
 *
 */
 
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <AutoConnect.h>

//==== config section ======================================

//-- read temp sensors every x milliseconds:
#define timer_millis 300

//-- define how many dallas sensors should be supported
#define maxNumDevices   8

// GPIO where the DS18B20 is connected to
#define GPIO_oneWireBus 4

//-- array of sensor addresses, plain and as hex string
struct SensorData {
  DeviceAddress devAddr;
  char      hexName[18];
  float         celsius;
  float      fahrenheit;
} ;
SensorData myDeviceData[maxNumDevices];

//-- counter for devices
int numberOfDevices;
int numberOfDevicesFound;

//==== global objects =========================================

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWireInst(GPIO_oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature dallas_sensors(&oneWireInst);

// Standard Webserver auf Port 80
ESP8266WebServer server;

// Das AutoConnect-Objekt, das den Server "umschließt"
AutoConnect portal(server);

//-- counter for "cron code"
int timer = 0;

//-- global buffer for converting device addresses to hex string
char hx[18];

String data_json = "";

//-- include files with support & handler functions
#include "tools.h"
#include "webhandlers.h"


//==== fire up your engines ... ====
void setup() {
  Serial.begin(115200);     // init serial
  Serial.println("\n********************\nMultiDallasSensorBox\n********************");

  dallas_sensors.begin();   // init dallas sensors
  find_sensor_addresses();  // build internal list of sensors

  Serial.println("Starte AutoConnect...");
  AutoConnectConfig config;            // Konfiguration (Optional)
  config.apid = "ESP8266-CFG-AP";      // Name des Access Points
  config.psk  = "12345678";            // Passwort für den AP (optional)
  config.title = "Mein ESP-Dashboard"; // Titel im Browser-Tab
  portal.config(config);


  //-- setup web server ressources
  server.on ( "/",           webhdl_Root );
  server.on ( "/styles.css", webhdl_CSS );
  server.on ( "/status",     webhdl_Status );
  server.onNotFound(webhdl_http404);

  //-- sensor data
  server.on ( "/sensor/addresses", webhdl_send_sensor_addresses );
  server.on ( "/sensor/all/json",  webhdl_send_sensor_all_json  );
  //-- individual sensor uris like /sensor/0x1234abcd/format
  for(int i=0; i<numberOfDevices; i++){
    server.on ( "/sensor/" + String(myDeviceData[i].hexName) + "/c" ,    webhdl_send_sensor_data );
    server.on ( "/sensor/" + String(myDeviceData[i].hexName) + "/f" ,    webhdl_send_sensor_data );
    server.on ( "/sensor/" + String(myDeviceData[i].hexName) + "/json" , webhdl_send_sensor_data );

  }

  // Start the portal which takes care of:
  // 1. Connection to wifi
  // 2. If that's not possible: Start the Access Point & Captive Portal
  if (portal.begin()) {
    Serial.println("WiFi verbunden: " + WiFi.localIP().toString());
  }

  //-- init timer for cronned function
  timer = millis();
}

//==== Run, Forrest, run! ====
void loop() {
  // IMPORTANT: Call handleClient() at beginning of loop() to keep
  // webserver and Captive Portal running
  portal.handleClient();

  //-- Add normal Sketch code here ...

  // every x milliseconds, read temp sensors
  if (millis() > timer + timer_millis) 
  {
    timer = millis(); // reset timer
    
    dallas_sensors.requestTemperatures();
    for(int i = 0; i < numberOfDevices; i++){
      float celsius = dallas_sensors.getTempC(myDeviceData[i].devAddr);
      myDeviceData[i].celsius    =  celsius;
      myDeviceData[i].fahrenheit = (celsius * 9.0 / 5.0) + 32.0 ;
      //- debug code: Serial.println("Sensor #" + String(i) + " : " + String(myDeviceData[i].celsius));
    }
  }
}



