/**************************************************************************************************************************************
  EEERover Starter Example
  
  Based on AdvancedWebServer.ino - Simple Arduino web server sample for SAMD21 running WiFiNINA shield
  For any WiFi shields, such as WiFiNINA W101, W102, W13x, or custom, such as ESP8266/ESP32-AT, Ethernet, etc
  
  WiFiWebServer is a library for the ESP32-based WiFi shields to run WebServer
  Forked and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Forked and modified from Arduino WiFiNINA library https://www.arduino.cc/en/Reference/WiFiNINA
  Built by Khoi Hoang https://github.com/khoih-prog/WiFiWebServer
  Licensed under MIT license
  
  Copyright (c) 2015, Majenko Technologies
  All rights reserved.
  
  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:
  
  Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.
  
  Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.
  
  Neither the name of Majenko Technologies nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.
  
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ***************************************************************************************************************************************/
#define USE_WIFI_NINA         false
#define USE_WIFI101           true
#include <WiFiWebServer.h>

const char ssid[] = "EEERover";
const char pass[] = "exhibition";
const int groupNumber = 13; // Set your group number to make the IP address constant - only do this on the EEERover network

//Webpage to return when root is requested
const char webpage[] = \
"<html><head><style>\
.btn {background-color: inherit;padding: 14px 28px;font-size: 16px;width: 14.2%; height: 50%}\
.btn:hover {background: #eee;}\
</style></head>\
<body>\
<button class=\"btn\" onclick=\"ledOn()\">Forward</button>\
<button class=\"btn\" onclick=\"ledOff()\">Stop</button>\
<button class=\"btn\" onclick=\"ledOnRight()\">Right</button>\
<button class=\"btn\" onclick=\"ledOnLeft()\">Left</button>\
<button class=\"btn\" onclick=\"ledOnReverse()\">Reverse</button>\
<button class=\"btn\" onclick=\"Speed()\">Speed</button>\
<button class=\"btn\" onclick=\"Scan()\">Scan</button>\
<br>Mineral: <span id=\"state\">Please Scan</span>\
</body>\
<script>\
var xhttp = new XMLHttpRequest();\
xhttp.onreadystatechange = function() {if (this.readyState == 4 && this.status == 200) {document.getElementById(\"state\").innerHTML = this.responseText;}};\
function ledOn() {xhttp.open(\"GET\", \"/on\"); xhttp.send();}\
function ledOff() {xhttp.open(\"GET\", \"/off\"); xhttp.send();}\
function ledOnRight() {xhttp.open(\"GET\", \"/right\"); xhttp.send();}\
function ledOnLeft() {xhttp.open(\"GET\", \"/left\"); xhttp.send();}\
function ledOnReverse() {xhttp.open(\"GET\", \"/reverse\"); xhttp.send();}\
function Speed() {xhttp.open(\"GET\", \"/speed\"); xhttp.send();}\
function Scan() {xhttp.open(\"GET\", \"/scan\"); xhttp.send();}\
</script></html>"; 

WiFiWebServer server(80);

//Return the web page
void handleRoot()
{
  server.send(200, F("text/html"), webpage);
}

int motorSpeed;
int time_on,time_off,counter,frequency, amplitude;
float period, voltage;
bool acoustic;

void ledON()
{  
  digitalWrite(9, 0);
  digitalWrite(12, 0);
  analogWrite(8,motorSpeed);
  analogWrite(11,motorSpeed);
  digitalWrite(LED_BUILTIN,1);
  server.send(200, F("text/plain"), F("ON"));
}

void OFF(){
  analogWrite(8,0);
  analogWrite(11,0);
  digitalWrite(LED_BUILTIN,0);
  server.send(200, F("text/plain"), F("Please Scan"));
}

void SCAN(){
  acoustic = false;
  voltage = analogRead(A0);
  Serial.write(" Magnetic Voltage: ");
  Serial.print(voltage);
  Serial.write("\n");
  time_on = pulseIn(6, HIGH, 10000);
  time_off = pulseIn(6, LOW, 10000);
  for(int i=0; i < 25; i++){
   amplitude = analogRead(A1);
   Serial.write("Amplitude: ");
   Serial.print(amplitude);
   Serial.write("\n");
   if(amplitude > 590 || amplitude < 470){
    i=25;
    acoustic = true;
    Serial.write("Acoustic\n");
   }
  }
  if(acoustic == false){
    Serial.write("Not Acoustic\n");
  }
  period = time_on+time_off;
  frequency = 1000000/period;
  Serial.write("Time On: ");
  Serial.print(time_on);
  Serial.write("\n");
  Serial.write("Time Off: ");
  Serial.print(time_off);
  Serial.write("\n");
  Serial.write("Frequency: ");
  Serial.print(frequency);
  Serial.write("\n");
  if(voltage < 521 || voltage > 524){
    if(frequency < 195){
      server.send(200, F("text/plain"), F("Adamantine"));
    }
    else if(frequency < 296){
      server.send(200, F("text/plain"), F("Xirang"));
    }
    else{
      server.send(200, F("text/plain"), F("Scan Again"));
    }
  }
  else{
    if(acoustic == true){
      if(frequency < 195 && frequency> 100){
        server.send(200, F("text/plain"), F("Gaborium"));
      }
      else{
        time_on = pulseIn(3, HIGH, 100000);
        time_off = pulseIn(3, LOW, 100000);
        period = time_on+time_off;
        frequency = 1000000/period;
        Serial.print(frequency);
        Serial.write("\n");
        if(frequency > 462 && frequency < 700){
          server.send(200, F("text/plain"), F("Netherite"));
        }
        else{
          server.send(200, F("text/plain"), F("Scan Again"));
      }
    }
    }
    else if(acoustic == false){
      if(frequency < 195){
        server.send(200, F("text/plain"), F("Adamantine"));
      }
      else if(frequency < 296){
        server.send(200, F("text/plain"), F("Lathwaite"));
      }
      else{
        time_on = pulseIn(3, HIGH, 100000);
        time_off = pulseIn(3, LOW, 100000);
        period = time_on+time_off;
        frequency = 1000000/period;
        Serial.print(frequency);
        Serial.write("\n");
        if(frequency < 462){
          server.send(200, F("text/plain"), F("Thiotimoline"));
        }
        else{
          server.send(200, F("text/plain"), F("Scan Again"));
        }
      }
    } 
 }
}

void ledONLEFT()
{  
  digitalWrite(9, 0);
  digitalWrite(12, 1);
  analogWrite(8,motorSpeed);
  analogWrite(11,motorSpeed);
  digitalWrite(LED_BUILTIN,1);
  server.send(200, F("text/plain"), F("ON"));
}

void ledONRIGHT()
{  
  digitalWrite(9, 1);
  digitalWrite(12, 0);
  analogWrite(8,motorSpeed);
  analogWrite(11,motorSpeed);
  digitalWrite(LED_BUILTIN,1);
  server.send(200, F("text/plain"), F("ON"));
}

void ledONREVERSE()
{  
  digitalWrite(9, 1);
  digitalWrite(12, 1);
  analogWrite(8,motorSpeed);
  analogWrite(11,motorSpeed);
  digitalWrite(LED_BUILTIN,1);
  server.send(200, F("text/plain"), F("ON"));
}


//Generate a 404 response with details of the failed request
void handleNotFound()
{
  String message = F("File Not Found\n\n"); 
  message += F("URI: ");
  message += server.uri();
  message += F("\nMethod: ");
  message += (server.method() == HTTP_GET) ? F("GET") : F("POST");
  message += F("\nArguments: ");
  message += server.args();
  message += F("\n");
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, F("text/plain"), message);
}

void SPEED(){
  if(motorSpeed==1000){
    motorSpeed=100;
  }
  else{
    motorSpeed=1000;
  }
  if((digitalRead(LED_BUILTIN)) == 1){
    analogWrite(8,motorSpeed);
    analogWrite(11,motorSpeed);
    server.send(200, F("text/plain"), F("ON"));
  }
  else{
    server.send(200, F("text/plain"), F("Please Scan"));
  }
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 0);
  pinMode(8,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(6,INPUT);
  pinMode(3,INPUT);
  pinMode(A0,INPUT);
  pinMode(A1, INPUT);
  motorSpeed = 1000;

  Serial.begin(9600);

  //Wait 10s for the serial connection before proceeding
  //This ensures y can see messages from startup() on the monitor
  //Remove this for faster startup when the USB host isn't attached
  while (!Serial && millis() < 10000);  

  Serial.println(F("\nStarting Web Server"));

  //Check WiFi shield is present
  if (WiFi.status() == WL_NO_SHIELD)
  {
    Serial.println(F("WiFi shield not present"));
    while (true);
  }

  //Configure the static IP address if group number is set
  if (13)
    WiFi.config(IPAddress(192,168,0,14));

  // attempt to connect to WiFi network
  Serial.print(F("Connecting to WPA SSID: "));
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED)
  {
    delay(500);
    Serial.print('.');
  }

  //Register the callbacks to respond to HTTP requests
  server.on(F("/"), handleRoot);
  server.on(F("/on"), ledON);
  server.on(F("/right"), ledONRIGHT);
  server.on(F("/left"), ledONLEFT);
  server.on(F("/reverse"), ledONREVERSE);
  server.on(F("/speed"), SPEED);
  server.on(F("/off"), OFF);
  server.on(F("/scan"), SCAN);

  server.onNotFound(handleNotFound);
  
  server.begin();
  
  Serial.print(F("HTTP server started @ "));
  Serial.println(static_cast<IPAddress>(WiFi.localIP()));
}

//Call the server polling function in the main loop
void loop()
{
  server.handleClient();
}
