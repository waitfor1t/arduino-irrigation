#include <NTPClient.h>
#include <WiFi.h>
#include <WebServer.h>
#include "index.h"
#include "config.h"
// #include <ESP8266mDNS.h> //Not sure I will use this. Comment out MDNS.begin("esp8266") to remove
// #include <MQTTClient.h> Will try this at some point

//new stuff to test
// This is an INPUT/OUTPUT test with webpage, get+arguments, and some EEPROM shit. There's more web server testing here. The index page uses a big string and PROGMEM.
// Has the "poo" page with test args.
// Mappings: 
// Where Do you want to send debug output?
#define DBG_OUTPUT_PORT Serial
#define DBG_OUTPUT_PORT_SPEED 115200
#define OPEN 0 //valve states
#define CLOSED 1
bool debug = true;                   // debug mode. true enables serial port and disables tx rx pins
/* Config files
const char* configFile = "/config.json";   // The config file name
const char* schedFile = "/sched.json";   // The sched file name6
const char* buttonsFile = "/buttons.json";   // The buttons file name
*/
#define membersof(x) (sizeof(x) / sizeof(x[0])) //macro to find members of array

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, (timeZone+DSTOffset)*60*60, 3600000); //3600000 = 1 hour, 60000 = 1 min update frequency
WebServer server(80);

void handleRoot() { //reads any number of arguments to set variables
  int len = membersof(zones); // sizeof(pins) / sizeof(pins[0]);
  String debug_page = "<!DOCTYPE HTML><html><head></head><body><h2>Debugging Page</h2><br>";
  debug_page += "URI: ";
  debug_page += server.uri();
  debug_page += " Method: ";
  debug_page += (server.method() == HTTP_GET) ? "GET" : "POST";
  debug_page += "\nArguments: ";
  debug_page += server.args();
  debug_page += "\n";
  // print server arguments, then set the DIO
  for (uint8_t i = 0; i < server.args(); ++i) {
    debug_page += " set " + server.argName(i) + "=" + server.arg(i) + " | ";
    digitalWrite(server.argName(i).toInt(), server.arg(i).toInt());
  };
  // print DIO states
  debug_page +="<br>Discrete I/O States:<br>";
  for(int i=0;i<len;i++) {
    debug_page += zones[i].Relay; debug_page += zones[i].label; debug_page += "-<a href='?"; debug_page += zones[i].GPIO;
    debug_page += "=0'>On</a>"; debug_page += "-<a href='?"; debug_page += zones[i].GPIO; debug_page += "=1'>Off</a>";
    debug_page += " State: "; debug_page += output_state[digitalRead(zones[i].GPIO)];
    debug_page += " Enabled?: "; debug_page += zones[i].enabled; debug_page += "<br>\n";
  }; 
  debug_page += "</body></html>";
  server.send(200, "text/html", index_html);
};
  /*
      String message;
    if (debug) { 
      DBG_OUTPUT_PORT.printf("#Args: %s",server.args()); 
 //     DBG_OUTPUT_PORT.println(server.args());
 // URL handler in debug mode. lets you set outputs from the URL directly using "http://server.home?12=1 where '12' is the GPIO#
      for (uint8_t i = 0; i < server.args(); i++) { 
        message += " " + server.argName(i); + " = " + server.arg(i); + "\n";
        DBG_OUTPUT_PORT.println(message);
        digitalWrite(server.argName(i).toInt(), server.arg(i).toInt());
      };
    };
    if (debug) DBG_OUTPUT_PORT.println(message);
    // now send the whole index page
    server.send(200, "text/html", index_html);
}
*/
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  
};
 // URL handler in debug mode. lets you set outputs from the URL directly using "http://server.home?12=1 where '12' is the GPIO#
void handleDebug() {
  int len = membersof(zones); // sizeof(pins) / sizeof(pins[0]);
  String debug_page = "<!DOCTYPE HTML><html><head></head><body><h2>Debugging Page</h2><br>";
  debug_page += "URI: ";
  debug_page += server.uri();
  debug_page += " Method: ";
  debug_page += (server.method() == HTTP_GET) ? "GET" : "POST";
  debug_page += "\nArguments: ";
  debug_page += server.args();
  debug_page += "\n";
  // print server arguments, then set the DIO
  for (uint8_t i = 0; i < server.args(); ++i) {
    debug_page += " set " + server.argName(i) + "=" + server.arg(i) + " | ";
    digitalWrite(server.argName(i).toInt(), server.arg(i).toInt());
  };
  // print DIO states
  debug_page +="<br>Discrete I/O States:<br>";
  for(int i=0;i<len;i++) {
    debug_page += zones[i].Relay; debug_page += zones[i].label; debug_page += "-<a href='?"; debug_page += zones[i].GPIO;
    debug_page += "=0'>On</a>"; debug_page += "-<a href='?"; debug_page += zones[i].GPIO; debug_page += "=1'>Off</a>";
    debug_page += " State: "; debug_page += output_state[digitalRead(zones[i].GPIO)];
    debug_page += " Enabled?: "; debug_page += zones[i].enabled; debug_page += "<br>\n";
  }; 
  debug_page += "</body></html>";
  server.send(200, "text/html", debug_page);
};

void handleConfig() {
/*  EEPROM.read(EEPROM_SSID);
  EEPROM.update(EEPROM_SSID); */
}
void runsequence() { //main sequencer using non-blocking millis
  unsigned long lastTime = millis();
  unsigned long now = lastTime;
  uint8_t numzones = membersof(zones);
  uint8_t runzones = membersof(zones);
  uint8_t i = 0;
  bool sent = false;

  if(debug) DBG_OUTPUT_PORT.printf("\Running Sequence...\n");
  i = 0;
  for (i = 0; i < numzones;) {
    //-------THIS IS WHERE WE OPEN THE VALVE NOTE: ESP32 "closed" = LOW, "open" = HIGH
    if (zones[i].enabled) {
      digitalWrite(zones[i].GPIO,OPEN); //open the valve
      yield();
      if (debug && !sent) DBG_OUTPUT_PORT.printf("Opening %s for %i minutes...",zones[i].label,zones[i].duration); 
      sent = true;
      now = millis();
      if (now - lastTime >= zones[i].duration*60 * 1000) { //we've exceeded the count, so reset lastTime, close, and jump to the next valve
       sent = false;
       lastTime = now;
       digitalWrite(zones[i].GPIO,CLOSED); //close the valve
       DBG_OUTPUT_PORT.printf("Closing %s\n", zones[i].label);
       i++;
      }; //if
    } else { if(debug) DBG_OUTPUT_PORT.printf("%s disabled\n",zones[i].label); i++;}; //if
  }; //for loop
  if(debug) DBG_OUTPUT_PORT.printf("\nSequence Done! %02u zones\n",numzones);
};
void setup() {
  int numtimes = membersof(starts);
  if(debug) DBG_OUTPUT_PORT.begin(DBG_OUTPUT_PORT_SPEED);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if(debug) DBG_OUTPUT_PORT.print(".");
  }
  // Server pages
  server.on("/", handleRoot);
  server.on("/debug", handleDebug);
  server.onNotFound(handleNotFound);
  server.begin();
  if(debug) {
    DBG_OUTPUT_PORT.printf("\nHello!\nConnected to %s, IP:",ssid); DBG_OUTPUT_PORT.println(WiFi.localIP()); //IP is already printable
    DBG_OUTPUT_PORT.print("  Initalizing outputs:");
  };
  // Initialize pins in pinArray as outputs
  int len = membersof(zones); // sizeof(pins) / sizeof(pins[0]);
  for(int i=0;i<len;i++) {
    pinMode(zones[i].GPIO,OUTPUT);
    digitalWrite(zones[i].GPIO,CLOSED); //set to off
    if(debug) DBG_OUTPUT_PORT.print(".");
  }; 
  if(debug) DBG_OUTPUT_PORT.println("Done!");
  timeClient.begin();
  if(debug) DBG_OUTPUT_PORT.printf("Starting Sprinkler on %s",schedule[timeClient.getDay()].day);

  if(debug) {
    DBG_OUTPUT_PORT.println(". HTTP server started!");
    DBG_OUTPUT_PORT.print(" Running today at "); for (uint8_t i=0; i<numtimes; i++) { if(starts[i].enabled) DBG_OUTPUT_PORT.printf("%02u:%02u ", starts[i].start_hr,starts[i].start_min);};
  };
};

void loop(void) {
  while(!timeClient.update()) {timeClient.forceUpdate(); } //force an update if you get a bad time
  uint8_t dd = timeClient.getDay();
  uint8_t hh = timeClient.getHours();
  uint8_t mm = timeClient.getMinutes();
  int numtimes = membersof(starts);
  bool start = false;
  yield();
  if (schedule[dd].enabled) { // are we running today? 
    for (uint8_t i=0; i<numtimes; i++) { //cycle through the times
      if (debug) start = true;
      else if (starts[i].enabled && starts[i].start_hr == hh && starts[i].start_min == mm && running = false) { // 'now' matches a start time
        // ***** SEQUENCER *****
        running = true;
        yield();
      };
    };
  };
  if (start) {
      if(debug) DBG_OUTPUT_PORT.printf("\Running Sequence...\n");
  i = 0;
  for (i = 0; i < numzones;) {
    //-------THIS IS WHERE WE OPEN THE VALVE NOTE: ESP32 "closed" = LOW, "open" = HIGH
    if (zones[i].enabled) {
      digitalWrite(zones[i].GPIO,OPEN); //open the valve
      yield();
      if (debug && !sent) DBG_OUTPUT_PORT.printf("Opening %s for %i minutes...",zones[i].label,zones[i].duration); 
      sent = true;
      now = millis();
      if (now - lastTime >= zones[i].duration*60 * 1000) { //we've exceeded the count, so reset lastTime, close, and jump to the next valve
       sent = false;
       lastTime = now;
       digitalWrite(zones[i].GPIO,CLOSED); //close the valve
       DBG_OUTPUT_PORT.printf("Closing %s\n", zones[i].label);
       i++;
      }; //if
    } else { if(debug) DBG_OUTPUT_PORT.printf("%s disabled\n",zones[i].label); i++;}; //if
  }; //for loop
  if(debug) DBG_OUTPUT_PORT.printf("\nSequence Done! %02u zones\n",numzones);
  };
  yield();
};

void loop1(void) {
  server.handleClient();
  yield();
};
