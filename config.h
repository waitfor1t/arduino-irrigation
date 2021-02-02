/* String days_arr[]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
bool days_enable[] = {1, 1, 1, 1, 1, 1, 1};
String times_arr[]={"08:07", "16:48", "22:49"};
bool times_enable[] = {1, 1, 1}; */
// Wifi setup
char ssid[32] = "chalkboard";        // This is the access point to connect to
char password[32] = "cebahoney";     // And its password
char host[32] = "sprinkler";        // The host name .local (mdns) hostname
// Timezone stuff
int8_t timeZone = -8;                    // Local timezone offset in hours, gets converted to seconds (8 = 28800 seconds)
int8_t DSTOffset = 0;                   // March 8, 2020=1, Nov 1, 2020=0 So effective TimeZone=TimeZone+DSTOffset
//EEPROM
#define EEPROM_SSID 0                 // where the ssid is kept in EEPROM
#define EEPROM_PASSWORD 32            // where the password is kept in EEPROM
#define EEPROM_HOST 64                 // where the host is kept in EEPROM
#define EEPROM_TIMEZONE 96          // where the host is kept in EEPROM
#define EEPROM_DSTOFFSET 97
// Days and states. change to language preference

const String output_state[]= {"On","Off"}; 
struct days { // schedule[i].day, schedule[i].enabled
  char* day;
  bool enabled;
} schedule [7] = { //run days
  {"Sunday", 1},
  {"Monday", 1},
  {"Tuesday", 0},
  {"Wednesday", 1},
  {"Thursday", 1},
  {"Friday", 1},
  {"Saturday", 1}
};
struct times { // times[i].enabled, times[i].start. Simplifies setup because start times are generally the same for all weekdays.
  uint8_t start_hr;
  uint8_t start_min;
  bool enabled;
} starts [3] = { //run start times
  {8, 07, 1},
  {16, 48, 1},
  {22, 49, 1}
};
// SET UP Zones consisting of a valve structures that map the valve to the relay and GPIO.

struct valve {
  char* label; //friendly label to show
  bool enabled; //1=enabled, 0=disabled
  uint8_t duration; //minutes
  uint8_t Relay; //the ID of the relay on the relay board
  uint8_t GPIO; //Internal GPIO reference
} zones[16] = { // zones[i].Name, zones[i].duration, zones[i].Relay, zone[i].GPIO,
  {"Zone1", 0, 1, 1, 13}, 
  {"Zone2", 0, 1, 2, 15},
  {"Zone3", 1, 1, 3, 12},
  {"Zone4", 0, 1, 4, 2},
  {"Zone5", 1, 1, 5, 14},
  {"Zone6", 0, 1, 6, 4},
  {"Zone7", 0, 1, 7, 27},
  {"Zone8", 0, 1, 8, 5},
  {"Zone9", 0, 1, 9, 26},
  {"Zone10", 0, 1, 10, 18},
  {"Zone11", 0, 1, 11, 25},
  {"Zone12", 0, 1, 12, 19},
  {"Zone13", 0, 1, 13, 33},
  {"Zone14", 0, 1, 14, 21},
  {"Zone15", 0, 1, 15, 32},
  {"Zone16", 0, 1, 16, 22}
}; 
char ntpServer[] = "pool.ntp.org"; // use as a variable
