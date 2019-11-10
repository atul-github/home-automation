// Author : Atul
// Home automation running on Wemos D1 Mini
// d2, d5, d6 and d7 are pins connected to physical switch
// while D0, D1, D3 and D8 connected to Relays and thus to light and fans

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const int pin=D1;
const char* ssid = "<SSID1>";
const char* ssid2 = "<SSID2>";
const char* ssid3 = "<SSID3>";

const char* password = "<password1>";
const char* password2 = "<password2>";
const char* password3 = "<password3>";

String IpAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ; 
}


int d2=0, d5=0, d6=0, d7=0;

// run ESP Server on Port 80
ESP8266WebServer server(80);
const int led = 13;
int status=1;
// Show WIFI Status when accessed root path
void handleRoot() {

  String msg = "Connected to : \n" ;
  msg += "SSID :";
  msg += WiFi.SSID();
  msg += "\n";

  IPAddress ip = WiFi.localIP();
  msg += "IP Address: ";
  msg += IpAddress2String(ip);
  msg += "\n";

  long rssi = WiFi.RSSI();
  
  msg += "Signal Strength : ";
  msg += String(rssi);
  if ( rssi > -30 ){
    msg += " (Amazing) ";
  }
  else if ( rssi > -67 ){
        msg += " (Very Good) ";
  }
  else if ( rssi > -70 ){
        msg += " (OK) ";
  }
  else if ( rssi > -80 ){
        msg += " (Not Good) ";
  }
  else{
    msg += " (Unstable)";
  }
  msg += "\n";
  server.send(200, "text/plain", msg);
//server.send(WiFi.localIP()
  return;
}

// Show message when no http path handler found
void handleNotFound(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void sendResponse(int p){
delay(10);
    int v = digitalRead(p);
  int status=v;
    String msg = "{ status : ";
    if ( status == HIGH ) 
      msg += "1 }";
    else
      msg += "0 }";
char s[50];
//strcpy(s, msg);
sprintf(s, "%d\0", v);
String str=s;
    server.send(200, "text/plain", str);
  
}

void toggle(int p){
if ( p == D2 || p == D5 || p == D6 || p == D7 ){
  sendResponse(p);
  return;
}
 
  digitalWrite(p, !digitalRead(p));

sendResponse(p);
}

void switchOnOff(int p, int v){

if ( p == D2 || p == D5 || p == D6 || p == D7 ){
  sendResponse(p);
  return;
}
    pinMode(p, OUTPUT);
    digitalWrite(p, v);

sendResponse(p);
}


// Try to connect to one of 3 WIFI
// Blink D4 doing So. 
void connectToWifi(){

digitalWrite(D4, HIGH);
  
    int counter=0;
    WiFi.begin(ssid, password);
      // Wait for connection
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        ++counter;
        if ( counter >= 20 ){
          break;
        }
      }

digitalWrite(D4, LOW);
delay(1000);
digitalWrite(D4, HIGH);
    
    if ( counter >= 20 && WiFi.status() != WL_CONNECTED ) {
      WiFi.begin(ssid2, password2);
    counter=0;
      while (WiFi.status() != WL_CONNECTED) {
        delay(400);
        ++counter;
        if ( counter >= 20 ){
          break;
        }
      }
    }

digitalWrite(D4, LOW);
delay(1000);
digitalWrite(D4, HIGH);
    
    if ( counter >= 20 && WiFi.status() != WL_CONNECTED ) {
      WiFi.begin(ssid3, password3);
    counter=0;
      while (WiFi.status() != WL_CONNECTED) {
        delay(400);
        ++counter;
        if ( counter >= 20 ){
          break;
        }
      }
    }
digitalWrite(D4, LOW);
}



void setup(void){
   
  Serial.begin(9600);

pinMode(D0, OUTPUT); // Light
pinMode(D1, OUTPUT); // Tube light
pinMode(D3, OUTPUT); // Hall Fan
pinMode(D4, OUTPUT); // Dining Light 
pinMode(D8, OUTPUT); // Dining Fan


for ( int x=0; x < 3; ++ x ){
  switchOnOff(D4, HIGH);
  delay(500);
  switchOnOff(D4, LOW);
  delay(500);
}

    digitalWrite(D0, 0);
    digitalWrite(D1, 0);
    digitalWrite(D3, 0);
    digitalWrite(D4, 0);
    digitalWrite(D8, 0);
    
pinMode(D2, INPUT_PULLUP);//
pinMode(D5, INPUT_PULLUP);
pinMode(D6, INPUT_PULLUP);
pinMode(D7, INPUT_PULLUP);

d2 = digitalRead(D2);
d5 = digitalRead(D5);
d6 = digitalRead(D6);
d7 = digitalRead(D7);

connectToWifi();


  //if (MDNS.begin("esp8266")) {
   // Serial.println("MDNS responder started");
  //}

  server.on("/switch/on/0", [](){
    switchOnOff(D0, 1);
  });
  server.on("/switch/on/1", [](){
    switchOnOff(D1, 1);
  });
  server.on("/switch/on/2", [](){
    switchOnOff(D2, 1);
  });
  server.on("/switch/on/3", [](){
    switchOnOff(D3, 1);
  });
  server.on("/switch/on/4", [](){
    switchOnOff(D4, HIGH);
  });
  server.on("/switch/on/5", [](){
    switchOnOff(D5, 1);
  });
  server.on("/switch/on/6", [](){
    switchOnOff(D6, 1);
  });
  server.on("/switch/on/7", [](){
    switchOnOff(D7, 1);
  });
  server.on("/switch/on/8", [](){
    switchOnOff(D8, 1);
  });


  server.on("/switch/off/0", [](){
    switchOnOff(D0, 0);
  });
  server.on("/switch/off/1", [](){
    switchOnOff(D1, 0);
  });
  server.on("/switch/off/2", [](){
    switchOnOff(D2, 0);
  });
  server.on("/switch/off/3", [](){
    switchOnOff(D3, 0);
  });
  server.on("/switch/off/4", [](){
    switchOnOff(D4, LOW);
  });
  server.on("/switch/off/5", [](){
    switchOnOff(D5, 0);
  });
  server.on("/switch/off/6", [](){
    switchOnOff(D6, 0);
  });
  server.on("/switch/off/7", [](){
    switchOnOff(D7, 0);
  });
  server.on("/switch/off/8", [](){
    switchOnOff(D8, 0);
  });

  server.on("/toggle/0", [](){
    toggle(D0);
  });
  server.on("/toggle/1", [](){
    toggle(D1);
  });
  server.on("/toggle/2", [](){
    toggle(D2);
  });
  server.on("/toggle/3", [](){
    toggle(D3);
  });
  server.on("/toggle/4", [](){
    toggle(D4);
  });
  server.on("/toggle/5", [](){
    toggle(D5);
  });
  server.on("/toggle/6", [](){
    toggle(D6);
  });
  server.on("/toggle/7", [](){
    toggle(D7);
  });
  server.on("/toggle/8", [](){
    toggle(D8);
  });


  server.on("/status/0", [](){
    sendResponse(D0);
  });
  server.on("/status/1", [](){
    sendResponse(D1);
  });
  server.on("/status/2", [](){
    sendResponse(D2);
  });
  server.on("/status/3", [](){
    sendResponse(D3);
  });
  server.on("/status/4", [](){
    sendResponse(D4);
  });
  server.on("/status/5", [](){
    sendResponse(D5);
  });
  server.on("/status/6", [](){
    sendResponse(D6);
  });
  server.on("/status/7", [](){
    sendResponse(D7);
  });
  server.on("/status/8", [](){
    sendResponse(D8);
  });
  
  
  server.on("/", handleRoot);
  
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

}

// This function read input on input PIns and set output to switch on light or fan.
// Input is connected to pin D2, D4, D6 and D7. while relay connected to D0, D1, D3 and D8
void setOutput(int pin, int pout, int &d){
int v = digitalRead(pin); // read switch PIN
if ( v ){
  if ( v != d ){
  if (!digitalRead(pout)) 
  digitalWrite(pout, 1);
  d=v;
  }
}
else {
  if ( v != d ){
    d=v;
  if ( digitalRead(pout) ) 
  digitalWrite(pout, 0);
  }
}
}

int wifiCounter=0;

void loop(void){

    if ( wifiCounter >= 500 ) {
      wifiCounter=0; 
      if ( WiFi.status() != WL_CONNECTED ){
        connectToWifi(); 
      }
    }
 
  ++wifiCounter;
 

  server.handleClient();

  setOutput(D2, D0, d2);
  setOutput(D5, D1, d5);
  setOutput(D6, D3, d6);
  setOutput(D7, D8, d7);
  
  delay(500);

  //yield();
}