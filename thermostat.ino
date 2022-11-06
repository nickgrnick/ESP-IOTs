/*
  This software, the ideas and concepts is Copyright (c) GSN 
  It started from an ideea of David Bird (http://dsbird.org.uk)
  All rights to this software are reserved.
  It is prohibited to redistribute or reproduce of any part or all of the software contents in any form other than the following:
  1. You may print or download to a local hard disk extracts for your personal and non-commercial use only.
  2. You may copy the content to individual third parties for their personal use, but only if you acknowledge the author David Bird as the source of the material.
  3. You may not, except with my express written permission, distribute or commercially exploit the content.
  4. You may not transmit it or store it in any other website or other form of electronic retrieval system for commercial purposes.
  5. You MUST include all of this copyright and permission notice ('as annotated') and this shall be included in all copies or substantial portions of the software
     and where the software use is visible to an end-user.
  THE SOFTWARE IS PROVIDED "AS IS" FOR PRIVATE USE ONLY, IT IS NOT FOR COMMERCIAL USE IN WHOLE OR PART OR CONCEPT.
  FOR PERSONAL USE IT IS SUPPLIED WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR
  A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHOR OR COPYRIGHT HOLDER BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OR
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
//--- 
#include <WiFi.h>                      // Built-in
#include <ESPmDNS.h>                   // Built-in
#include <SPIFFS.h>                    // Built-in
#include "Wire.h"                      // Built-in
#include <ESP32Time.h>                 // https://www.arduinolibraries.info/libraries/esp32-time

#include <WiFiClient.h>
#include <WiFiAP.h>

#include "ESPAsyncWebServer.h"         // https://github.com/me-no-dev/ESPAsyncWebServer/tree/63b5303880023f17e1bca517ac593d8a33955e94
#include "AsyncTCP.h"                  // https://github.com/me-no-dev/AsyncTCP

#include "DHT.h"                       // Written by Adafruit Industries
//using BME280 sensor
/*
#include <Adafruit_Sensor.h>           // Adafruit sensor
#include <Adafruit_BME280.h>           // For BME280 support
*/
#include <ArduinoJson.h>               // https://arduinojson.org

#include "html.h"

//using BME280 sensor
/*
Adafruit_BME280 BME280_sensor;         // I2C mode
#define SensorAddress   0x76           // Use 0x77 for an Adafruit variant
*/
#define DHTTYPE DHT22 // DHT 22 (AM2302), AM2321
//using DHT Sensor;
#define DHT_PIN         13
DHT DHT_sensor(DHT_PIN, DHTTYPE);      //VCC 3.3V

#define SensorReadings  300            // maximum number of sensor readings, typically 250/day at 6-per-hour
#define NumOfSensors    2              // number of sensors (+1), set by the graphing section
#define NumOfEvents     4              // Number of events per-day, 4 is a practical limit

#define ON              true           // Set the Relay ON
#define OFF             false          // Set the Relay OFF

#define Relay_PIN       16            // Define the Relay Control pin
#define RelayReverse    false         // Set to true for Relay that requires a signal LOW for ON

#define ReadHeat_PIN    34            // Define the heater read temp pin

#define Heat_LED_PIN    17            // Define the LED heater Control pin
#define Wait_LED_PIN    18            // Define the LED standby Control pin

#define simulating      OFF           // Switch OFF for actual sensor readings, ON for simulated random values

String serverMode = "UNK";

typedef struct {
  float amb_Temp = 0.0;
  float heat_Temp = 0.0;  
  float trgt_Temp = 0.0;  
  byte  amb_Humi = 0;
} sensorData;

sensorData sensors_data[NumOfSensors][SensorReadings];

struct settings {
  String DoW;                // Day of Week for the programmed event
  String Start[NumOfEvents]; // Start time
  String Stop[NumOfEvents];  // End time
  String Temp[NumOfEvents];  // Required temperature during the Start-End times
};

String       Received_Data[10];                  // TxId, RxId, MsgCnt, Temperature, Humidity, RelayState, Incoming Msg, Msg Rssi, Msg SNR (10-fields are sent to this Rx)
String       SensorReading[NumOfSensors][6];     // 254 Sensors max. and 6 Parameters per sensor T, H, Relay-state. Maximum LoRa adress range is 255 - 1 for Server so 0 - 253

String       setup_file = "settings.json";       // Setup storage file name on flash

String       Date_str, Time_str, DoW_str;        // For Date and Time
settings     dows_sched[7];                      // Schedule settings, 7-days of the week
int          SensorReadingPointer[NumOfSensors]; // Used for sensor data storage

float ThermoPower = 100.0;                      // heater power
float Hysteresis = 0.2;                         // Heating Hysteresis default value 

//################ VARIABLES ################
const char* wifi_ssid = "";           // WiFi SSID
const char* wifi_pass = "";           // WiFi Password

int TimeZone = 0; // put in settings (a select for choice wright settings save / load)         // put in settings
String TimesZones[9] = {
  "MET-1METDST,M3.5.0/01,M10.5.0/02", // West of Europe
  "CET-1CEST,M3.5.0,M10.5.0/3",       // Central Europe
  "EST-2METDST,M3.5.0/01,M10.5.0/02", // Est of Europe
  "EST5EDT,M3.2.0,M11.1.0",           // EST USA
  "CST6CDT,M3.2.0,M11.1.0",           // CST USA
  "MST7MDT,M4.1.0,M10.5.0",           // MST USA
  "NZST-12NZDT,M9.5.0,M4.1.0/3",      // Auckland
  "EET-2EEST,M3.5.5/0,M10.5.5/0",     // Asia
  "ACST-9:30ACDT,M10.1.0,M4.1.0/3"    // Australia  
};

// System values
String DeviceName           = "The Thermostat";
String DeviceType           = "thermostat";
String ServerName           = "thermostat"; // Connect to the server with http://hpserver.local/ e.g. if name = "myserver" use http://myserver.local/

//################  VERSION  ###########################################
String VersYear             = "MMXXII";   // For the footer line
String VersNo               = "1.0.beta";      // Programme version, see change log at end

//################ VARIABLES ###########################################
float  AmbTemp              = 0.0;        // Variable for the current ambient temperature
float  HeatTemp             = 0.0;        // Variable for the current heater temperature
float  AmbHumid             = 0.0;        // Variable for the current ambient humidity

float  TargetTemp           = 22.0;       // Default thermostat value for set temperature

float  FrostTemp            = 5;          // Default thermostat value for frost protection temperature

float  MaxAmbTemp           = 26.0;       // Maximum temperature detection, switches off thermostat when reached
float  MaxHeatTemp          = 38.0;       // Maximum heater temperature, prevent heater over-heating

bool   ManOverride          = false;      // Manual override
float  ManOverrideTemp      = 21.0;       // Manual override temperature

int    EarlyStart           = 0;          // Default thermostat value for early start of heating

String RelayState           = "OFF";      // Current setting of the control/thermostat relay
String SchedState           = "OFF";      // Current setting of the timer

String TUnits               = "C";        // or Units

String html_page            = "";         // General purpose variable to hold HTML code for display

int    TimerCheckDuration   = 5000;       // Check for timer event every 5-seconds
int    DataReadingInterval  = 10;         // Add sensor reading every N-mins

int    LastTimerSwitchCheck = 0;          // Counter for last timer check
int    LastReadingCheck     = 0;          // Counter for last reading saved check
float  LastAmbTemp          = 0.0;        // Last temperature used for rogue reading detection

int UnixTime               = 0;           // Time now (when updated) of the current time

bool RTC_ON                 = false;      // Real Time Computer is functioning or not
ESP32Time RTC;                            // offset in seconds GMT+0

String login_user = "thermostat";
String login_pass = "changemepls";

String token;                             // is an random number used to secure POST-ing informtions

String dev_IP;                            // the IP alocated to the device by LAN DHCP or fixed (192.168.4.1) in this code when the device run as Access Point

unsigned int http_Port = 80;              // web port for accesing the device web-server
AsyncWebServer webServer(http_Port); // Server on IP address port 80 (web-browser default, change to your requirements, e.g. 8080
// To access server from outside of a WiFi (LAN) network e.g. on port 8080 add a rule on your Router that forwards a connection request
// to http://your_WAN_address:8080/ to http://your_LAN_address:8080 and then you can view your ESP server from anywhere.
// Example http://yourhome.ip:8080 and your ESP Server is at 192.168.0.40, then the request will be directed to http://192.168.0.40:8080

ARDUINOJSON_NAMESPACE::DynamicJsonDocument devices_JSON(2048);
ARDUINOJSON_NAMESPACE::DynamicJsonDocument settings_JSON(4096);

//---
void setup() {
  SetupSystem();                          // General system setup

  StartSPIFFS();                          // Start SPIFFS filing system

  token = String(random(99999999, 1000000000)); // generate the session token value
  Serial.print("This is token key : ");
  Serial.println(token);

  Initialise_Array();                     // Initialise the array for storage and set some values
  RecoverSettings();                      // Recover settings from LittleFS

  StartWiFi();                            // Start WiFi services

  RTC_ON = SetupTime();                   // Start NTP clock services. When it succed the device run with net sincronized clock, otherwise it function with value seted by user from browser
  
  StartSensor();
  ReadSensor();                                           // Get current sensor values
  for (word r = 0; r < SensorReadings; r++) {
    sensors_data[1][r].amb_Temp = AmbTemp;
    sensors_data[1][r].heat_Temp = HeatTemp;
    sensors_data[1][r].trgt_Temp = TargetTemp;
    sensors_data[1][r].amb_Humi = AmbHumid;
  }
  //ActuateHeating(OFF);                                    // Switch heating OFF
  //ReadSensor();                                           // Get current sensor values
  LastTimerSwitchCheck = millis() + TimerCheckDuration;   // preload timer value with update duration

  //this structure, together with other resources from the application, will be used in the future for an ingenious auto-integration of several devices present in the same LAN
  devices_JSON[0]["name"] = DeviceName;
  devices_JSON[0]["type"] = DeviceType;
  devices_JSON[0]["url"] = dev_IP;
  devices_JSON[0]["port"] = http_Port;
}
//---
bool checkRequest(AsyncWebServerRequest * request) {
  char c_user[login_user.length() + 1];
  login_user.toCharArray(c_user, login_user.length() + 1);

  char c_pass[login_pass.length() + 1];
  login_pass.toCharArray(c_pass, login_pass.length() + 1);

  if(!request->authenticate((const char*)c_user, (const char*)c_pass)) {
    request->requestAuthentication();
    return false;
  } else {
    if (RTC_ON) {
      return true;
    } else {
      request->redirect("/rtc");
      return false;
    }
  }
}
//---
void webServerRouting() {
  webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (checkRequest(request)) {
      request->redirect("/home");
    }
  });
  webServer.on("/js", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasArg("lib")) {
      String strArg = request->arg("lib");
      request->send(SPIFFS, "/" + strArg, "text/javascript");
    } else {
      Forb_page();
      request->send(200, "text/html", html_page);    
    }
  });
  // Set handler for '/forb'
  webServer.on("/forb", HTTP_GET, [](AsyncWebServerRequest * request) {
      Forb_page();
      request->send(200, "text/html", html_page);
  });
  // Set handler for '/home'
  webServer.on("/home", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (checkRequest(request)) {
      Home_page();
      request->send(200, "text/html", html_page);
    }
  });
  // Set handler for '/graphs'
  webServer.on("/graphs", HTTP_GET, [](AsyncWebServerRequest * request)   {
    if (checkRequest(request)) {
      Graphs_page();
      request->send(200, "text/html", html_page);
    }
  });
  // Set handler for '/sched'
  webServer.on("/sched", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (checkRequest(request)) {
      Sched_page();
      request->send(200, "text/html", html_page);
    }
  });
  // Set handler for '/setup'
  webServer.on("/setup", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (checkRequest(request)) {
      Setup_page();
      request->send(200, "text/html", html_page);
    }
  });
  // Set handler for '/help'
  webServer.on("/help", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (checkRequest(request)) {
      Help_page();
      request->send(200, "text/html", html_page);
    }
  });
  // Set handler for '/ifrm'
  webServer.on("/ifrm", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (request->hasArg("dev-name") && request->hasArg("dev-url")) {
      String html_iframe;
      
      html_iframe = String(iframe_html);
      html_iframe.replace("[css-style]", String(css_style));
      html_iframe.replace("[dev-name]", String(request->arg("dev-name")));
      html_iframe.replace("[dev-url]", String(request->arg("dev-url")));
  
      html_iframe.replace("[funs-after-load-doc]", String(funs_after_load_doc));

      String strJSON = "";
      ARDUINOJSON_NAMESPACE::serializeJson(devices_JSON, strJSON);
      html_iframe.replace("[devices]", String(strJSON));

      request->send(200, "text/html", html_iframe);
    } else {
      request->redirect("/home");
    }
  });
  //
  webServer.on("/rtc", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (!RTC_ON) {
      SetRTC_page();
      request->send(200, "text/html", html_page);      
    } else {
      request->redirect("/");
    }
  });
  //
  webServer.on("/ssid", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (checkRequest(request)) {
      SetSSID_page();
      request->send(200, "text/html", html_page);
    }
  });
  //
  webServer.on("/post-ssid", HTTP_POST, [](AsyncWebServerRequest * request) {
    if (request->hasArg("token") && String(request->arg("token")).equals(token)) {
      String wifi_ssid_str = "";
      String wifi_pass_str = "";
      if (request->hasArg("wifi-ssid") && request->hasArg("wifi-pass")) {
        wifi_ssid_str = request->arg("wifi-ssid");
        wifi_ssid_str.trim();
        wifi_pass_str = request->arg("wifi-pass");
        wifi_pass_str.trim();

        File setup_data = SPIFFS.open("/" + setup_file, "w");
        if (setup_data) {
          Serial.println("Saving settings...");
        
          settings_JSON["wifi-ssid"] = wifi_ssid_str;
          settings_JSON["wifi-pass"] = wifi_pass_str;

          String settings_str = "";
          ARDUINOJSON_NAMESPACE::serializeJson(settings_JSON, settings_str);
          setup_data.println(settings_str);

          setup_data.close();
        }
        request->redirect("/"); 
        
        delay(5000);

        ESP.restart();
      }
    }
  });  
  webServer.on("/post-rtc", HTTP_POST, [](AsyncWebServerRequest * request) {
    //if (checkRequest(request)) {
      if (request->hasArg("token") && String(request->arg("token")).equals(token)) {
        if (request->hasArg("date-time-local")) {
          String strArg = request->arg("date-time-local");

          int year = strArg.substring(0, 4).toInt();
          int month = strArg.substring(5, 7).toInt();
          int day = strArg.substring(8, 10).toInt();
          int hour = strArg.substring(11, 13).toInt();
          int min = strArg.substring(14, 16).toInt();
          int sec = 0;

          Serial.print("Date & time before : ");
          Serial.println(RTC.getDateTime());

          RTC.setTime(sec, min, hour, day, month, year);

          Serial.print("Date & time after : ");
          Serial.println(RTC.getDateTime());
          
          RTC_ON = true;

          request->redirect("/"); 
        }
      }
    //}
  });
  // Set handler for '/post-sched' inputs
  webServer.on("/post-sched", HTTP_POST, [](AsyncWebServerRequest * request) {
    if (checkRequest(request)) {
      if (request->hasArg("token") && String(request->arg("token")).equals(token)) {
        for (byte dow = 0; dow < 7; dow++) {
          for (byte p = 0; p < NumOfEvents; p++) {
            dows_sched[dow].Temp[p]  = request->arg(String(dow) + "-" + String(p) + "-temp");
            dows_sched[dow].Start[p] = request->arg(String(dow) + "-" + String(p) + "-start");
            dows_sched[dow].Stop[p]  = request->arg(String(dow) + "-" + String(p) + "-stop");
          }
        }
        SaveSettings();
        request->redirect("/home");
      } else {
        request->redirect("/forb");
      }
    }
  });
  // Set handler for '/post-setup' inputs
  webServer.on("/post-setup", HTTP_POST, [](AsyncWebServerRequest * request) {
    if (checkRequest(request)) {
      if (request->hasArg("token") && String(request->arg("token")).equals(token)) {
        if (request->hasArg("dev-name")) {
          String strArg = request->arg("dev-name");
          DeviceName = strArg;
        }
        if (request->hasArg("srv-name")) {
          String strArg = request->arg("srv-name");
          ServerName = strArg;
        }        
        if (request->hasArg("login-user")) {
          String strArg = request->arg("login-user");
          login_user = strArg;
        }
        if (request->hasArg("login-pass")) {
          String strArg = request->arg("login-pass");
          login_pass = strArg;
        }        
        if (request->hasArg("thermo-power")) {
          String numArg = request->arg("thermo-power");
          ThermoPower = numArg.toFloat();
        }
        if (request->hasArg("hysteresis")) {
          String numArg = request->arg("hysteresis");
          Hysteresis = numArg.toFloat();
        }
        if (request->hasArg("frost-temp")) {
          String numArg = request->arg("frost-temp");
          FrostTemp = numArg.toFloat();
        }
        if (request->hasArg("early-start")) {
          String numArg = request->arg("early-start");
          EarlyStart = numArg.toInt();
        }
        if (request->hasArg("manual-override")) {
          String strArg = request->arg("manual-override");
          ManOverride = (strArg == "ON") ? ON : OFF;
        }
        if (request->hasArg("manual-override-temp")) {
          String numArg   = request->arg("manual-override-temp");
          ManOverrideTemp = numArg.toFloat();
        }
        if (request->hasArg("time-zone")) {
          String numArg = request->arg("time-zone");
          TimeZone = numArg.toInt();
        }
        if (request->hasArg("max-ambient-temp")) {
          String numArg = request->arg("max-ambient-temp");
          MaxAmbTemp = numArg.toFloat();
        }
        if (request->hasArg("max-heater-temp")) {
          String numArg = request->arg("max-heater-temp");
          MaxHeatTemp = numArg.toFloat();
        }
        if (request->hasArg("temp-units")) {
          String strArg = request->arg("temp-units");
          TUnits = strArg;
        }
        SaveSettings();
        request->redirect("/home");
      } else {
        request->redirect("/forb");
      }
    }
  });
  // Set handler for AJAX call '/fresh-data'
  webServer.on("/fresh-data", HTTP_POST, [](AsyncWebServerRequest * request) {
    if (request->hasArg("token") && String(request->arg("token")).equals(token)) {
      request->send(200, "text/plain", momentValuesJSON());
    } else {
      request->redirect("/forb");
    }
  });
  // Set handler for AJAX call '/fresh-draw'
  webServer.on("/fresh-draw", HTTP_POST, [](AsyncWebServerRequest * request) {
    if (request->hasArg("token") && String(request->arg("token")).equals(token)) {
      request->send(200, "text/plain", momentValuesJSON_ex());
    } else {
      request->redirect("/forb");
    }
  });  
  // Set handler for AJAX call '/fresh-wifi'
  webServer.on("/fresh-wifi", HTTP_POST, [](AsyncWebServerRequest * request) {
    if (request->hasArg("token") && String(request->arg("token")).equals(token)) {
      request->send(200, "text/plain", momentWifiPercentJSON());
    } else {
      request->redirect("/forb");
    }
  });
  //search for other device in LAN - TCP network
  webServer.on("/search", HTTP_GET, [](AsyncWebServerRequest * request) {
    //if (request->hasArg("token") && String(request->arg("token")).equals(emb_token)) {
      InitLANSearchDevices();     
      request->send(200, "text/html", html_page);
    //} else {
      //request->redirect("/forb");
    //}
  });  
  //answer for request device comunications
  webServer.on("/who-im", HTTP_POST, [](AsyncWebServerRequest * request) {
    //if (checkRequest(request)) {
      AsyncWebServerResponse *response;
      response = request->beginResponse(200, "text/plain", whoImValuesJSON());
      response->addHeader("Access-Control-Allow-Origin", "http://");
      response->addHeader("Vary", "Origin");
      response->addHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
      response->addHeader("Access-Control-Allow-Methods", "POST, GET");
      //response->addHeader("Content-Type", "application/json");

      request->send(response);
    //}
  });  
}

//---
void loop() {
  if (RTC_ON) {
    if ((millis() - LastTimerSwitchCheck) > TimerCheckDuration) {
      LastTimerSwitchCheck = millis();                      // Reset time
      ReadSensor();                                         // Get sensor readings, or get simulated values if 'simulated' is ON
      UpdateLocalTime();                                    // Updates Time UnixTime to 'now'
      CheckTimerEvent();                                    // Check for schedules actuated
    }
    if ((millis() - LastReadingCheck) > (DataReadingInterval * 60 * 1000)) {
      LastReadingCheck = millis();                          // Update reading record every ~n-mins e.g. 60,000uS = 1-min
      AssignSensorReadingsToArray();
    }
  }
}
//---
String ChartTData() { // make data JSON string for temperatures chart
  //"[['Hour', 'Rm T°', 'Tgt T°'], 
  word r = 0;
  String ret;
  ret = "[";
  ret += "[\"Hour\", \"amb °" + TUnits + "\", \"heat °" + TUnits + "\", \"tgt °" + TUnits + "\"]";
  do {
    ret += ",[" + 
      String(r) + "," + 
      String(sensors_data[1][r].amb_Temp, 1) + "," + 
      String(sensors_data[1][r].heat_Temp, 1) + "," + 
      String(sensors_data[1][r].trgt_Temp, 1) + 
      "]";
    r++;
  } while (r < SensorReadings);
  ret += "]";

  return ret;
}
//---
String ChartHData() { // make data JSON string for humidity chart
  //"[['Hour', 'RH %'], 

  word r = 0;
  String ret;
  ret = "[";
  ret += "[\"Hour\", \"RH %\"]";
  do {
    ret += ",[" + String(r) + "," + String(sensors_data[1][r].amb_Humi) + "]";
    r++;
  } while (r < SensorReadings);
  ret += "]";

  return ret;
}
//---
String WiFiPercent() {
  float Signal = WiFi.RSSI();
  Signal = 90 / 40.0 * Signal + 212.5; // From Signal = 100% @ -50dBm and Signal = 10% @ -90dBm and y = mx + c
  if (Signal > 100) {
    Signal = 100;
  }
  return String(Signal, 0) + "%";
}
//---
String getThermoTime(){
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return "--/--/---- --:--";
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

  char buff[24];
  sprintf(buff, "%s, %02d/%02d/%04d, %02d:%02d", 
      dows_sched[timeinfo.tm_wday].DoW, timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900, timeinfo.tm_hour, timeinfo.tm_min);

  return String(buff);
}
//---
String getWorkMode() {
  String wm = "monitor";
  if (SchedState == "ON") {
    wm = "program";
  } else {
    if (ManOverride) {
      wm = "manual";
    }
  }
  return wm;
}
//---
String momentValuesJSON() {
  String ret;
  ret = "{";
  ret += "\"temp-ambient\":\"" + String(AmbTemp, 1) + String("°") + TUnits + "\",";
  ret += "\"temp-target\":\"" + String(TargetTemp, 1) + String("°") + TUnits + "\",";
  ret += "\"temp-heater\":\"" + String(HeatTemp, 1) + String("°") + TUnits + "\",";
  ret += "\"humid-ambient\":\"" + String(AmbHumid, 0) + String("%") + "\",";
  ret += "\"relay-state\":\"" + String((RelayState == "ON") ? "on" : "off") + "\",";
  ret += "\"work-mode\":\"" + getWorkMode() + "\",";
  ret += "\"thermo-clock\":\"" + getThermoTime() + "\"";
  ret += "}";
  return ret;
}
//---
String whoImValuesJSON() {
  String ret;
  ret = "{";
  ret += "\"name\":\"" + String(DeviceName) + "\",";
  ret += "\"type\":\"" + String(DeviceType) + "\",";
  ret += "\"url\":\"" + dev_IP + "\",";
  ret += "\"port\":\"" + String(http_Port) + "\"";
  ret += "}";
  
  return ret;
}
//---
String momentValuesJSON_ex() {
  String ret;
  ret = "{";
  ret += "\"temp-ambient\":\"" + String(AmbTemp, 1) + String("°") + TUnits + "\",";
  ret += "\"temp-target\":\"" + String(TargetTemp, 1) + String("°") + TUnits + "\",";
  ret += "\"temp-heater\":\"" + String(HeatTemp, 1) + String("°") + TUnits + "\",";
  ret += "\"humid-ambient\":\"" + String(AmbHumid, 0) + String("%") + "\",";
  ret += "\"relay-state\":\"" + String((RelayState == "ON") ? "on" : "off") + "\",";
  ret += "\"work-mode\":\"" + getWorkMode() + "\",";
  ret += "\"thermo-clock\":\"" + getThermoTime() + "\",";
  ret += "\"dt\":" + ChartTData() + ",";
  ret += "\"dh\":" + ChartHData() + "";
  ret += "}";
  return ret;
}
//---
String momentWifiPercentJSON() {
  String ret;
  ret = "{";
  ret += "\"wifi-perc\":\"" + WiFiPercent() + "\"";
  ret += "}";
  return ret;
}
//---
void Home_page() {
  HTML_header();
  html_page += String(menu_html);
  String ret;
  ret = String(info_html);
  ret.replace("[status]", String(status_html));
  html_page += ret;
  JS_AjaxStatusTimer();
  JS_AjaxWifiTimer();
  HTML_footer(true);
}
//---
void InitLANSearchDevices() {
  HTML_header();
  html_page += String(menu_html);
  String ret;
  ret = String(search_html);
  ret.replace("[user]", String(login_user));
  ret.replace("[pass]", String(login_pass));
  html_page += ret;

  HTML_footer(false);
}
//---
void Forb_page() {
  HTML_header();
  html_page += String(menu_html);
  html_page += String(forb_html);
  JS_AjaxWifiTimer();
  HTML_footer(false);
}
//---
void JS_OnDocReady() {
  String ret;
  ret = String(OnDocReady_html);
  ret.replace("[funs-after-load-doc]", String(funs_after_load_doc));
  String strJSON = "";
  ARDUINOJSON_NAMESPACE::serializeJson(devices_JSON, strJSON);
  ret.replace("[devices]", String(strJSON));
  html_page += ret;
}
//---
void JS_AjaxStatusTimer() {
  String ret;
  ret = String(StatusTimer_html);
  ret.replace("[token]", String(token));
  ret.replace("[timer-check-duration]", String(TimerCheckDuration));
  html_page += ret;
}
//---
void JS_AjaxWifiTimer() {
  String ret;
  ret = String(WifiTimer_html);
  ret.replace("[token]", String(token));
  ret.replace("[timer-check-duration]", String(TimerCheckDuration));
  html_page += ret;
}
//---
void JS_AjaxChartsTimer() {
  String ret;
  ret = String(ChartsTimer_html);
  ret.replace("[token]", String(token));
  ret.replace("[temp-units]", String(TUnits));
  ret.replace("[timer-check-duration]", String(TimerCheckDuration));
  html_page += ret;
}
//---
void Sched_page() {
  HTML_header();

  html_page += String(menu_html);

  String vals_table;
  vals_table =    "<table class=\"center w-95\">";
  vals_table +=      "<col><col><col><col><col><col><col><col>";
  vals_table +=      "<tr><td>Control</td>";
  for (byte dow = 1; dow <= 6; dow++) { // Heading line showing DoW
    vals_table +=      "<td>" + dows_sched[dow].DoW + "</td>";
  }
  vals_table +=        "<td>" + dows_sched[0].DoW + "</td>"; //
  vals_table +=      "</tr>";
  for (byte p = 0; p < NumOfEvents; p++) {
    vals_table +=    "<tr>";
    vals_table +=      "<td>Temp</td>";
    for (int dow = 1; dow <= 6; dow++) {
      vals_table +=    "<td><input type=\"text\" name=\"" + String(dow) + "-" + String(p) + "-temp\" value=\"" + dows_sched[dow].Temp[p] + "\" maxlength=\"5\" size=\"5\"></td>";
    }
    vals_table +=      "<td><input type=\"text\" name=\"" + String(0) + "-" + String(p) + "-temp\" value=\""  + dows_sched[0].Temp[p] + "\" maxlength=\"5\" size=\"6\"></td>"; //
    vals_table +=    "</tr>";

    vals_table +=    "<tr>";
    vals_table +=      "<td>Start</td>";
    for (int dow = 1; dow <= 6; dow++) {
      vals_table +=    "<td><input type=\"time\" name=\"" + String(dow) + "-" + String(p) + "-start\" value=\"" + dows_sched[dow].Start[p] + "\"></td>";
    }
    vals_table +=      "<td><input type=\"time\" name=\"" + String(0) + "-" + String(p) + "-start\" value=\"" + dows_sched[0].Start[p] + "\"></td>"; //
    vals_table +=    "</tr>";

    vals_table +=    "<tr>";
    vals_table +=      "<td>Stop</td>";
    for (int dow = 1; dow <= 6; dow++) {
      vals_table +=    "<td><input type=\"time\" name=\"" + String(dow) + "-" + String(p) + "-stop\" value=\"" + dows_sched[dow].Stop[p] + "\"></td>";
    }
    vals_table +=      "<td><input type=\"time\" name=\"" + String(0) + "-" + String(p) + "-stop\" value=\"" + dows_sched[0].Stop[p] + "\"></td>"; //
    vals_table +=    "</tr>";

    vals_table +=    "<tr>";
    vals_table +=      "<td colspan=\"8\"><hr/></td>";
    vals_table +=    "</tr>";
  }
  vals_table +=      "</table>";

  String ret;
  ret = String(sched_html);
  ret.replace("[token]", String(token));
  ret.replace("[vals-table]", String(vals_table));
  html_page += ret;
  
  JS_AjaxWifiTimer();

  HTML_footer(true);
}
//---
void Graphs_page() {
  HTML_header();
  
  html_page += String(menu_html);

  String ret;
  ret = String(graph_html);
  ret.replace("[status]", String(status_html));
  html_page += ret;

  JS_AjaxChartsTimer();
  JS_AjaxWifiTimer();

  HTML_footer(true);
}
//---
void Setup_page() {
  HTML_header();
  
  html_page += String(menu_html);

  String ret;
  ret = String(setup_html);
  ret.replace("[token]", String(token));
  ret.replace("[dev-name]", String(DeviceName));
  ret.replace("[srv-name]", String(ServerName));
  ret.replace("[login-user]", String(login_user));
  ret.replace("[login-pass]", String(login_pass));
  ret.replace("[thermo-power]", String(ThermoPower, 0));  

  ret.replace("[time-zone-selected-0]", String(TimeZone == 0 ? "selected" : ""));
  ret.replace("[time-zone-selected-1]", String(TimeZone == 1 ? "selected" : ""));
  ret.replace("[time-zone-selected-2]", String(TimeZone == 2 ? "selected" : ""));
  ret.replace("[time-zone-selected-3]", String(TimeZone == 3 ? "selected" : ""));
  ret.replace("[time-zone-selected-4]", String(TimeZone == 4 ? "selected" : ""));
  ret.replace("[time-zone-selected-5]", String(TimeZone == 5 ? "selected" : ""));
  ret.replace("[time-zone-selected-6]", String(TimeZone == 6 ? "selected" : ""));
  ret.replace("[time-zone-selected-7]", String(TimeZone == 7 ? "selected" : ""));
  ret.replace("[time-zone-selected-8]", String(TimeZone == 8 ? "selected" : ""));

  ret.replace("[hysteresis]", String(Hysteresis, 1));  
  ret.replace("[frost-temp]", String(FrostTemp, 1));  
  ret.replace("[early-start]", String(EarlyStart));  

  ret.replace("[manual-override-selected-on]", String((ManOverride == ON) ? "selected" : ""));
  ret.replace("[manual-override-selected-off]", String((ManOverride == OFF) ? "selected" : ""));

  ret.replace("[manual-override-temp]", String(ManOverrideTemp, 1));
  ret.replace("[max-ambient-temp]", String(MaxAmbTemp, 1));
  ret.replace("[max-heater-temp]", String(MaxHeatTemp, 1));

  ret.replace("[temp-unit-selected-c]", String((TUnits == "C") ? "selected" : ""));  
  ret.replace("[temp-unit-selected-f]", String((TUnits == "F") ? "selected" : ""));  
  html_page += ret;
  JS_AjaxWifiTimer();
  HTML_footer(true);
}
//---
void Help_page() {
  HTML_header();
  html_page += String(menu_html);
  html_page += String(help_html);
  JS_AjaxWifiTimer();
  HTML_footer(true);
}
//---
void SetRTC_page() {
  HTML_header();
  String ret = String(set_rtc_html);
  ret.replace("[token]", String(token));
  html_page += ret;
  JS_AjaxWifiTimer();
  HTML_footer(true);
}
//---
void SetSSID_page() {
  HTML_header();
  html_page += String(menu_html);
  String ret = String(set_ssid_html);
  ret.replace("[token]", String(token));
  ret.replace("[wifi-ssid]", String(wifi_ssid));
  ret.replace("[wifi-pass]", String(wifi_pass));
  html_page += ret;
  JS_AjaxWifiTimer();
  HTML_footer(true);
}
//---
void CheckTimerEvent() {
  String TimeNow;
  UpdateTargetTemperature();
  TimeNow = ConvertUnixTime(UnixTime);                  // Get the current time e.g. 15:35
  SchedState = "OFF";                                      // Switch timer off until decided by the schedule
  if (EarlyStart > 0) {                                    // If early start is enabled by a value > 0
    TimeNow = ConvertUnixTime(UnixTime + EarlyStart * 60); // Advance the clock by the Early Start Duration
  }
  if (ManOverride == ON) {                              // If manual override is enabled then turn the heating on
    TargetTemp = ManOverrideTemp;                          // Set the target temperature to the manual overide temperature
    ControlHeating();                                      // Control the heating as normal
  } else {
    for (byte dow = 0; dow < 7; dow++) {                     // Look for any valid timer events, if found turn the heating on
      for (byte p = 0; p < NumOfEvents; p++) {
        // Now check for a scheduled ON time, if so Switch the Timer ON and check the temperature against target temperature
        if (String(dow) == DoW_str && (TimeNow >= dows_sched[dow].Start[p] && TimeNow <= dows_sched[dow].Stop[p] && dows_sched[dow].Start[p] != "")) {
          SchedState = "ON";
          ControlHeating();
          ManOverride = OFF; // If it was ON turn it OFF when the timer starts a controlled period
        }
      }
    }
  }
  CheckAndSetFrostTemperature();
}
//---
void ControlHeating() {
  if ((AmbTemp <= (TargetTemp - Hysteresis)) && (HeatTemp <= MaxHeatTemp)) { // Check if room temeperature is below set-point and hysteresis offset and not overheat heater
    ActuateHeating(ON);
  } else
  if ((AmbTemp >= (TargetTemp + Hysteresis)) || (AmbTemp >= MaxAmbTemp) || (HeatTemp > MaxHeatTemp)) { // Check if room temeperature is above set-point and hysteresis offset and not overheat heater
    ActuateHeating(OFF);                                   // Switch Relay/Heating OFF if so
  }
}
//---
void UpdateTargetTemperature() {
  String TimeNow;
  TimeNow = ConvertUnixTime(UnixTime);
  for (byte dow = 0; dow < 7; dow++) {
    for (byte p = 0; p < NumOfEvents; p++) {
      if (String(dow) == DoW_str && (TimeNow >= dows_sched[dow].Start[p] && TimeNow < dows_sched[dow].Stop[p])) {
        TargetTemp = dows_sched[dow].Temp[p].toFloat();               // Found the programmed set-point temperature from the scheduled time period
      }
    }
  }
  if (ManOverride == ON) {
    TargetTemp = ManOverrideTemp;
  }
  Serial.println("Target Temperature : " + String(TargetTemp, 1) + String(" °") + TUnits);
}
//---
void CheckAndSetFrostTemperature() {
  if (SchedState == "OFF" && ManOverride == OFF) {
    if (AmbTemp < (FrostTemp - Hysteresis)) {
      ActuateHeating(ON);
      Serial.println("Frost protection actuated...");
    }
    if (AmbTemp > (FrostTemp + Hysteresis)) {
      ActuateHeating(OFF);
    }
  }
}
//---
void ActuateHeating(bool demand) {
  if (demand) {
    RelayState = "ON";
    //set leds states
    digitalWrite(Heat_LED_PIN, HIGH);
    digitalWrite(Wait_LED_PIN, LOW);
    //set relay heater
    if (RelayReverse) {
      digitalWrite(Relay_PIN, HIGH);
    } else {
      digitalWrite(Relay_PIN, LOW);
    }
    //serial print info thermostat state
    Serial.println("Thermostat ON");
  } else {
    RelayState = "OFF";
    //set leds states
    digitalWrite(Heat_LED_PIN, LOW);
    digitalWrite(Wait_LED_PIN, HIGH);
    //disconect relay heater
    if (RelayReverse) {
      digitalWrite(Relay_PIN, LOW);
    } else {
      digitalWrite(Relay_PIN, HIGH);
    }
    //serial print info thermostat state
    Serial.println("Thermostat OFF");
  }
}
//---
float readThermistorTemp(uint8_t the_pin) { // this code is a simple implementation of 10kohm termistor
/*
    TR0 = thermistor 10Kohm
    R01 = normal rezistor 10Kohm

    +3.3v ---[R01=10kohm]---+---[TR0=10kom]----+
                            |                  |
    ReadHeat_PIN (GPIO34) __|             GND _|_
*/
  uint16_t b_coeficient = 3950;
  uint16_t r_nom_value = 10000; 
  uint16_t r_this_value = 10000;
  float t_nom_value = 25.0;
  
  uint8_t n_read = 5;
  uint16_t samples[n_read];

  uint8_t i;
  float r_average;

  for (i = 0; i < n_read; i++) {
   samples[i] = analogRead(the_pin);
   delay(10);
  }
  
  // average all the samples out
  r_average = 0;
  for (i = 0; i < n_read; i++) {
     r_average += samples[i];
  }
  r_average /= n_read;
  Serial.print("Average analog reading "); 
  Serial.println(r_average);
  
  // convert the value to resistance
  r_average = 4095 / r_average - 1;
  r_average = r_this_value / r_average;
  Serial.print("Thermistor resistance "); 
  Serial.println(r_average);
  
  float steinhart;
  steinhart = r_average / r_nom_value;
  steinhart = log(steinhart);
  steinhart /= b_coeficient;
  steinhart += 1.0 / (t_nom_value + 273.15);
  steinhart = 1.0 / steinhart;
  steinhart -= 273.15;
  
  return steinhart;
}
//---
void ReadSensor() {
  if (simulating) { // code for simulating mode
    AmbTemp   = 20.2 + random(-15, 15) / 10.0; // Generate a random temperature value between 18.7° and 21.7°
    HeatTemp   = 25.0 + random(-15, 50) / 10.0; // Generate a random temperature value between 18.7° and 21.7°
    AmbHumid  = random(45, 55);                // Generate a random humidity value between 45% and 55%
  } else {
    HeatTemp = readThermistorTemp(ReadHeat_PIN);
    uint8_t i;
    /*
    //BME280_sensor
    while (isnan(BME280_sensor.readTemperature())) { }  // Make sure there are no reading errors
    AmbTemp = BME280_sensor.readTemperature();      // Read the current temperature
    //end BME280_sensor
    */
    //DHT_sensor
    i = 0;
    while (isnan(DHT_sensor.readTemperature(TUnits.equals("F"))) || (i < 3)) { i++; }  // Make sure there are no reading errors
    AmbTemp = DHT_sensor.readTemperature(TUnits.equals("F"));
    //end DHT_sensor
    if (AmbTemp >= 50 || AmbTemp < -30) {
      AmbTemp = LastAmbTemp; // Check and correct any errorneous readings
    }
    LastAmbTemp = AmbTemp;
    /*
    //BME280_sensor
    while (isnan(BME280_sensor.readHumidity())) { }     // Make sure there are no reading errors
    AmbHumid = BME280_sensor.readHumidity();
    //end BME280_sensor
    */
    //DHT_sensor
    i = 0;
    while (isnan(DHT_sensor.readHumidity()) || (i < 3)) { i++; }     // Make sure there are no reading errors
    AmbHumid = DHT_sensor.readHumidity();
    //end DHT_sensor    
    Serial.println("Ambient temperature : " + String(AmbTemp, 1) + ", Heater temperature : " + String(HeatTemp, 1) + ", Ambient humidity : " + String(AmbHumid, 0) + String(" °") + TUnits);
  }

  Serial.print("IP >>> ");
  Serial.println(dev_IP + ":" + http_Port);
}
//---
void AssignSensorReadingsToArray() {
  SensorReading[1][0] = 1;
  SensorReading[1][1] = AmbTemp;
  SensorReading[1][2] = HeatTemp;
  SensorReading[1][3] = AmbHumid;
  SensorReading[1][4] = RelayState;
  AddReadingToSensorData(1, AmbTemp, HeatTemp, TargetTemp, AmbHumid); // Only sensor-1 is implemented here, could  be more though
}
//---
void AddReadingToSensorData(byte grp_ID, float amb_Temp, float heat_Temp, float trgt_Temp, byte amb_Humid) {
  word ptr, p;
  ptr = SensorReadingPointer[grp_ID];
  sensors_data[grp_ID][ptr].amb_Temp = amb_Temp;
  sensors_data[grp_ID][ptr].heat_Temp = heat_Temp;
  sensors_data[grp_ID][ptr].trgt_Temp = trgt_Temp;
  sensors_data[grp_ID][ptr].amb_Humi = amb_Humid;
  ptr++;
  if (ptr >= SensorReadings) {
    p = 0;
    do {
      sensors_data[grp_ID][p].amb_Temp  = sensors_data[grp_ID][p + 1].amb_Temp;
      sensors_data[grp_ID][p].heat_Temp  = sensors_data[grp_ID][p + 1].heat_Temp;
      sensors_data[grp_ID][p].trgt_Temp  = sensors_data[grp_ID][p + 1].trgt_Temp;
      sensors_data[grp_ID][p].amb_Humi  = sensors_data[grp_ID][p + 1].amb_Humi;
      p++;
    } while (p < SensorReadings);
    ptr = SensorReadings - 1;
    sensors_data[grp_ID][SensorReadings - 1].amb_Temp = amb_Temp;
    sensors_data[grp_ID][SensorReadings - 1].heat_Temp = heat_Temp;
    sensors_data[grp_ID][SensorReadings - 1].trgt_Temp = trgt_Temp;
    sensors_data[grp_ID][SensorReadings - 1].amb_Humi = amb_Humid;
  }
  SensorReadingPointer[grp_ID] = ptr;
}
//---
void HTML_header() {
  html_page = String(header_html);
  html_page.replace("[dev-name]", String(DeviceName));
  html_page.replace("[css-style]", String(css_style));
  html_page.replace("[funs-after-load-doc]", String(funs_after_load_doc));
}
//---
void HTML_footer(bool fresh) {
  if (fresh) {
    JS_OnDocReady();
  }

  String ret;
  ret = String(footer_html);
  ret.replace("[vers-year]", String(VersYear));
  ret.replace("[vers-no]", String(VersNo));
  html_page += ret;
}
//---
void SetServerName() {
  const char * theName;
  theName = ServerName.c_str(); 

  if (serverMode.equals("WEB")) {
    if (MDNS.begin((char *)theName)) { // The name that will identify your device on the network
      Serial.print("MDNS responder started as : http://");
      Serial.print(theName);
      Serial.println(".local");
      MDNS.addService("n8i-mlp", "tcp", 23); // Add service
    } else
      Serial.println("Error setting up MDNS responder...");
  }
}
//---
void checkWifiConnect() {
  if (!WiFi.isConnected()) {
    StartWiFi();
  }
}
//---
void StartWiFi() {
  Serial.print("\r\nTry connecting to : "); 
  Serial.println(String(wifi_ssid));

  WiFi.disconnect();
  WiFi.mode(WIFI_STA);       // switch off AP

  WiFi.begin(wifi_ssid, wifi_pass);

  uint8_t n_try = 0;
  uint8_t m_try = 20;

  Serial.print("[");
  while ((WiFi.status() != WL_CONNECTED) && (n_try < m_try)) {
    n_try++;
    Serial.print(".");
    delay(500);
  }
  if (WiFi.isConnected()) {
    Serial.println("] - connected");

    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);

    serverMode = "WEB";

    dev_IP = WiFi.localIP().toString();
    Serial.println("WiFi connected at : " + dev_IP);
    
    IPAddress dns(8, 8, 8, 8); // Google as DNS
    if (dns.fromString(WiFi.gatewayIP().toString())) {
      Serial.println("WiFi gateway => DNS : " + WiFi.gatewayIP().toString());
    }
    Serial.println("WiFi gateway : " + WiFi.gatewayIP().toString());
  } else {
    Serial.println("] - NOT connected");

    WiFi.disconnect();
    WiFi.mode(WIFI_MODE_AP);

    serverMode = "AP";

    Serial.println("Configuring as Access Point...");

    char ssid_name[DeviceType.length() + 1];
    String ap_name = DeviceType + "-" + ServerName;
    ap_name.toCharArray(ssid_name, ap_name.length() + 1);
    char ssid_pass[login_pass.length() + 1];
    login_pass.toCharArray(ssid_pass, login_pass.length() + 1);

    WiFi.softAP((char *)ssid_name, (char *)ssid_pass);

    delay(1000);
    // Set static IP
    IPAddress AP_LOCAL_IP(192, 168, 4, 1);
    IPAddress AP_GATEWAY_IP(192, 168, 4, 2);
    IPAddress AP_NETWORK_MASK(255, 255, 255, 0);
    if (WiFi.softAPConfig(AP_LOCAL_IP, AP_GATEWAY_IP, AP_NETWORK_MASK)) {
      dev_IP = WiFi.softAPIP().toString();
      Serial.print("AP IP address : ");
      Serial.println(dev_IP);
      Serial.println("Acces Point started...");    
    } else {
      Serial.println("AP Config Failed...");
    }
  }    

  SetServerName();
      
  webServerRouting();
  webServer.begin();
}
//---
void SetupSystem() {
  Serial.begin(115200);                                           // Initialise serial communications
  delay(200);
  Serial.println(__FILE__);
  Serial.println("Starting...");

  pinMode(Relay_PIN, OUTPUT);
  pinMode(Heat_LED_PIN, OUTPUT);
  pinMode(Wait_LED_PIN, OUTPUT);

  pinMode(ReadHeat_PIN, INPUT);

  //put pins off
  if (RelayReverse) {
    digitalWrite(Relay_PIN, LOW);
  } else {
    digitalWrite(Relay_PIN, HIGH);
  }

  digitalWrite(Wait_LED_PIN, LOW);
  digitalWrite(Heat_LED_PIN, HIGH);
}
//---
bool SetupTime() {
  if (serverMode.equals("WEB")) {
    configTime(0, 0, "time.nist.gov");                               // (gmtOffset_sec, daylightOffset_sec, ntpServer)
    char cTZ[TimesZones[TimeZone].length() + 1];
    TimesZones[TimeZone].toCharArray(cTZ, TimesZones[TimeZone].length() + 1);
    setenv("TZ", (char*)cTZ, 1);                                     // setenv()adds "TZ" variable to the environment, only used if set to 1, 0 means no change
    tzset();
    delay(200);
    return UpdateLocalTime();
  } else {
    return false;
  }
}
//---
bool UpdateLocalTime() {
  struct tm timeinfo;
  time_t now;
  char time_output[30];
  while (!getLocalTime(&timeinfo, 15000)) {                        // Wait for up to 15-sec for time to synchronise
    return false;
  }
  time(&now);
  UnixTime = now;
  
  Serial.print("Unix time : ");
  Serial.println(UnixTime);

  //See http://www.cplusplus.com/reference/ctime/strftime/
  strftime(time_output, sizeof(time_output), "%d/%B/%Y", &timeinfo);  //
  Date_str = time_output;  
  strftime(time_output, sizeof(time_output), "%H:%M", &timeinfo);     // Creates: '14:05'
  Time_str = time_output;
  strftime(time_output, sizeof(time_output), "%w", &timeinfo);        // Creates: '0' for Sun
  DoW_str = time_output;
  return true;
}
//---
String ConvertUnixTime(int unix_time) {
  time_t tm = unix_time;
  struct tm *now_tm = localtime(&tm);
  char output[40];
  strftime(output, sizeof(output), "%H:%M", now_tm);               // Returns 21:12
  return output;
}
//---
void StartSPIFFS() {
  Serial.println("Starting SPIFFS");
  boolean SPIFFS_Status;
  SPIFFS_Status = SPIFFS.begin();
  if (SPIFFS_Status == false) {
    Serial.println("Formatting SPIFFS (it may take some time)...");
    SPIFFS.begin(true); // Now format SPIFFS
    SPIFFS.format();
  }
  else {
    Serial.println("SPIFFS Started successfully...");
  }
}
//---
void Initialise_Array() {
  dows_sched[0].DoW = "Sun"; 
  dows_sched[1].DoW = "Mon"; 
  dows_sched[2].DoW = "Tue"; 
  dows_sched[3].DoW = "Wed"; 
  dows_sched[4].DoW = "Thu"; 
  dows_sched[5].DoW = "Fri"; 
  dows_sched[6].DoW = "Sat";
}
//---
void SaveSettings() {
  Serial.println("Getting ready to Save settings...");
  File setup_data = SPIFFS.open("/" + setup_file, "w");
  if (setup_data) { // Save settings
    Serial.println("Saving settings...");
    for (byte dow = 0; dow < 7; dow++) {
      Serial.println("Day of week = " + String(dow));
      for (byte p = 0; p < NumOfEvents; p++) {
        settings_JSON["sched"][dow][p]["temp"] = String(dows_sched[dow].Temp[p]);
        settings_JSON["sched"][dow][p]["start"] = String(dows_sched[dow].Start[p]);
        settings_JSON["sched"][dow][p]["stop"] = String(dows_sched[dow].Stop[p]);

        Serial.println("Period : " + String(p) + " " + dows_sched[dow].Temp[p] + " from: " + dows_sched[dow].Start[p] + " to: " + dows_sched[dow].Stop[p]);
      }
    }
    settings_JSON["dev-name"] = String(DeviceName);
    settings_JSON["srv-name"] = String(ServerName);
    settings_JSON["login-user"] = String(login_user);
    settings_JSON["login-pass"] = String(login_pass);
    settings_JSON["thermo-power"] = String(ThermoPower);
    settings_JSON["time-zone"] = String(TimeZone);
    settings_JSON["hysteresis"] = String(Hysteresis, 1);
    settings_JSON["frost-temp"] = String(FrostTemp, 1);
    settings_JSON["early-start"] = String(EarlyStart);
    settings_JSON["man-override"] = String(ManOverride);
    settings_JSON["man-override-temp"] = String(ManOverrideTemp, 1);
    settings_JSON["max-ambient-temp"] = String(MaxAmbTemp, 1);
    settings_JSON["max-heater-temp"] = String(MaxHeatTemp, 1);
    settings_JSON["temp-units"] = String(TUnits);

    String settings_str = "";
    ARDUINOJSON_NAMESPACE::serializeJson(settings_JSON, settings_str);
    setup_data.println(settings_str);
    setup_data.close();
    
    SetupTime();
    devices_JSON[0]["name"] = DeviceName;

    Serial.println("Settings saved ...");
    Serial.println(" Thermostat name : " + DeviceName);
    Serial.println(" Server name : " + ServerName);
    Serial.println(" Login user name : " + login_user);
    Serial.println(" Login password : ***********");
    Serial.println(" Thermostat power (W) : " + String(ThermoPower));
    Serial.println(" Timezone : " + TimesZones[TimeZone]);
    Serial.println(" Hysteresis : " + String(Hysteresis, 1));
    Serial.println(" Frost temperature : " + String(FrostTemp, 1));
    Serial.println(" Early start : " + String(EarlyStart));
    Serial.println(" Manual override : " + String(ManOverride));
    Serial.println(" Manual override temperature : " + String(ManOverrideTemp));
    Serial.println(" Maximum ambient temperature : " + String(MaxAmbTemp, 1));
    Serial.println(" Maximum heater temperature : " + String(MaxHeatTemp, 1));
    Serial.println(" Units for temperature : " + TUnits);
  }
}
//
void RecoverSettings() {
  //String Entry;
  Serial.println("Reading settings...");
  File setup_data = SPIFFS.open("/" + setup_file, "r");
  if (setup_data) { // if the file is available, read it
    Serial.println("Recovering settings...");
    String settings_str = "";
    while (setup_data.available()) {
      settings_str = setup_data.readStringUntil('\n');
    }

    ARDUINOJSON_NAMESPACE::deserializeJson(settings_JSON, settings_str);
    for (byte dow = 0; dow < 7; dow++) {
      Serial.println("Day of week = " + String(dow));
      for (byte p = 0; p < NumOfEvents; p++) {
        dows_sched[dow].Temp[p] = (settings_JSON["sched"][dow][p]["temp"].as<String>() != "null") ? settings_JSON["sched"][dow][p]["temp"].as<String>() : "";
        dows_sched[dow].Start[p] = (settings_JSON["sched"][dow][p]["start"].as<String>() != "null") ? settings_JSON["sched"][dow][p]["start"].as<String>() : "";
        dows_sched[dow].Stop[p] = (settings_JSON["sched"][dow][p]["stop"].as<String>() != "null") ? settings_JSON["sched"][dow][p]["stop"].as<String>() : "";

        Serial.println("Period: " + String(p) + " " + dows_sched[dow].Temp[p] + " from: " + dows_sched[dow].Start[p] + " to: " + dows_sched[dow].Stop[p]);
      }
    }

    DeviceName = (settings_JSON["dev-name"].as<String>() != "null") ? settings_JSON["dev-name"].as<String>() : DeviceName;
    ServerName = (settings_JSON["srv-name"].as<String>() != "null") ? settings_JSON["srv-name"].as<String>() : ServerName;
    login_user = (settings_JSON["login-user"].as<String>() != "null") ? settings_JSON["login-user"].as<String>() : login_user;
    login_pass = (settings_JSON["login-pass"].as<String>() != "null") ? settings_JSON["login-pass"].as<String>() : login_pass;
    ThermoPower = (settings_JSON["thermo-power"].as<String>() != "null") ? settings_JSON["thermo-power"].as<String>().toFloat() : ThermoPower;
    TimeZone = (settings_JSON["time-zone"].as<String>() != "null") ? settings_JSON["time-zone"].as<String>().toInt() : TimeZone;
    Hysteresis = (settings_JSON["hysteresis"].as<String>() != "null") ? settings_JSON["hysteresis"].as<String>().toFloat() : Hysteresis;
    FrostTemp = (settings_JSON["frost-temp"].as<String>() != "null") ? settings_JSON["frost-temp"].as<String>().toFloat() : FrostTemp;
    EarlyStart = (settings_JSON["early-start"].as<String>() != "null") ? settings_JSON["early-start"].as<String>().toInt() : EarlyStart;
    ManOverride = (settings_JSON["man-override"].as<String>() != "null") ? settings_JSON["man-override"].as<String>().toInt() : ManOverride;
    ManOverrideTemp = (settings_JSON["man-override-temp"].as<String>() != "null") ? settings_JSON["man-override-temp"].as<String>().toFloat() : ManOverrideTemp;
    MaxAmbTemp = (settings_JSON["max-ambient-temp"].as<String>() != "null") ? settings_JSON["max-ambient-temp"].as<String>().toFloat() : MaxAmbTemp;
    MaxHeatTemp = (settings_JSON["max-heater-temp"].as<String>() != "null") ? settings_JSON["max-heater-temp"].as<String>().toFloat() : MaxHeatTemp;
    TUnits = (settings_JSON["temp-units"].as<String>() != "null") ? settings_JSON["temp-units"].as<String>() : TUnits;

    String tempStr_1 = (settings_JSON["wifi-ssid"].as<String>() != "null") ? settings_JSON["wifi-ssid"].as<String>() : "";
    tempStr_1.trim();
    wifi_ssid = tempStr_1.c_str();

    String tempStr_2 = (settings_JSON["wifi-pass"].as<String>() != "null") ? settings_JSON["wifi-pass"].as<String>() : "";
    tempStr_2.trim();
    wifi_pass = tempStr_2.c_str();

  }
  setup_data.close();

  devices_JSON[0]["name"] = DeviceName;
      
  Serial.println("Settings recovered...");
  Serial.println(" Thermostat name : " + DeviceName);
  Serial.println(" Server name : " + ServerName);
  Serial.println(" Login user name : " + login_user);
  Serial.println(" Login password : ***********");
  Serial.println(" Thermostat power (W) : " + String(ThermoPower));
  Serial.println(" Timezone : " + TimesZones[TimeZone]);
  Serial.println(" Hysteresis : " + String(Hysteresis, 1));
  Serial.println(" Frost temperature : " + String(FrostTemp, 1));
  Serial.println(" Early start : " + String(EarlyStart));
  Serial.println(" Manual override : " + String(ManOverride));
  Serial.println(" Manual override temperature : " + String(ManOverrideTemp, 1));
  Serial.println(" Maximum ambient temperature : " + String(MaxAmbTemp, 1));
  Serial.println(" Maximum heater temperature : " + String(MaxHeatTemp, 1));
  Serial.println(" Units for temperature : " + TUnits);
}
//---
void StartSensor() {
  /*
  //BME280_sensor
  Wire.setClock(100000);                           // Slow down the SPI bus for some BME280 devices
  if (!simulating) {                               // If not sensor simulating, then start the real one
    bool status = sensor.begin(SensorAddress);     // You can also pass a Wire library object like &Wire2, e.g. status = bme.begin(0x76, &Wire2);
    if (!status) {
      Serial.println("Could not find a valid BME280 sensor, check wiring, address and sensor ID!");
      Serial.print("SensorID is: 0x"); Serial.println(sensor.sensorID(), 16);
      Serial.print("       ID of 0xFF probably means a bad address, or a BMP 180 or BMP 085 or BMP280\n");
      Serial.print("  ID of 0x56-0x58 represents a BMP 280,\n");
      Serial.print("       ID of 0x60 represents a BME 280.\n");
    } else {
      Serial.println("Sensor started...");
    }
    delay(1000);                                   // Wait for sensor to start
  }
  //end BME280_sensor
  */
  //DHT_sensor
  pinMode(DHT_PIN, INPUT);
  DHT_sensor.begin();
  //end DHT_sensor
  //read heater temp

  //end read heater temp
}
