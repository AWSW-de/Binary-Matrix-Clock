// ###########################################################################################################################################
// #
// # Code for the printables Binary Matrix Clock project:
// # https://www.printables.com/de/model/558337-binary-matrix-clock
// #
// # Code by https://github.com/AWSW-de
// #
// # Released under licenses:
// # GNU General Public License v3.0: https://github.com/AWSW-de/Binary-Matrix-Clock/blob/main/LICENSE and
// # Creative Commons Attribution-NonCommercial 3.0 Unported License http://creativecommons.org/licenses/by-nc/3.0
// # Important: NonCommercial — You may not use the material for commercial purposes !
// #
// ###########################################################################################################################################
/*

      ___                       ___           ___           ___           ___                    ___           ___           ___           ___                       ___                    ___           ___       ___           ___           ___     
     /\  \          ___        /\__\         /\  \         /\  \         |\__\                  /\__\         /\  \         /\  \         /\  \          ___        |\__\                  /\  \         /\__\     /\  \         /\  \         /\__\    
    /::\  \        /\  \      /::|  |       /::\  \       /::\  \        |:|  |                /::|  |       /::\  \        \:\  \       /::\  \        /\  \       |:|  |                /::\  \       /:/  /    /::\  \       /::\  \       /:/  /    
   /:/\:\  \       \:\  \    /:|:|  |      /:/\:\  \     /:/\:\  \       |:|  |               /:|:|  |      /:/\:\  \        \:\  \     /:/\:\  \       \:\  \      |:|  |               /:/\:\  \     /:/  /    /:/\:\  \     /:/\:\  \     /:/__/     
  /::\~\:\__\      /::\__\  /:/|:|  |__   /::\~\:\  \   /::\~\:\  \      |:|__|__            /:/|:|__|__   /::\~\:\  \       /::\  \   /::\~\:\  \      /::\__\     |:|__|__            /:/  \:\  \   /:/  /    /:/  \:\  \   /:/  \:\  \   /::\__\____ 
 /:/\:\ \:|__|  __/:/\/__/ /:/ |:| /\__\ /:/\:\ \:\__\ /:/\:\ \:\__\     /::::\__\          /:/ |::::\__\ /:/\:\ \:\__\     /:/\:\__\ /:/\:\ \:\__\  __/:/\/__/ ____/::::\__\          /:/__/ \:\__\ /:/__/    /:/__/ \:\__\ /:/__/ \:\__\ /:/\:::::\__\
 \:\~\:\/:/  / /\/:/  /    \/__|:|/:/  / \/__\:\/:/  / \/_|::\/:/  /    /:/~~/~             \/__/~~/:/  / \/__\:\/:/  /    /:/  \/__/ \/_|::\/:/  / /\/:/  /    \::::/~~/~             \:\  \  \/__/ \:\  \    \:\  \ /:/  / \:\  \  \/__/ \/_|:|~~|~   
  \:\ \::/  /  \::/__/         |:/:/  /       \::/  /     |:|::/  /    /:/  /                     /:/  /       \::/  /    /:/  /         |:|::/  /  \::/__/      ~~|:|~~|               \:\  \        \:\  \    \:\  /:/  /   \:\  \          |:|  |    
   \:\/:/  /    \:\__\         |::/  /        /:/  /      |:|\/__/     \/__/                     /:/  /        /:/  /     \/__/          |:|\/__/    \:\__\        |:|  |                \:\  \        \:\  \    \:\/:/  /     \:\  \         |:|  |    
    \::/__/      \/__/         /:/  /        /:/  /       |:|  |                                /:/  /        /:/  /                     |:|  |       \/__/        |:|  |                 \:\__\        \:\__\    \::/  /       \:\__\        |:|  |    
     ~~                        \/__/         \/__/         \|__|                                \/__/         \/__/                       \|__|                     \|__|                  \/__/         \/__/     \/__/         \/__/         \|__|    

*/


// ###########################################################################################################################################
// # Includes:
// #
// # You will need to add the following libraries to your Arduino IDE to use the project:
// # - Adafruit NeoPixel      // by Adafruit:                     https://github.com/adafruit/Adafruit_NeoPixel
// # - AsyncTCP               // by me-no-dev:                    https://github.com/me-no-dev/AsyncTCP
// # - ESPAsyncWebServer      // by me-no-dev:                    https://github.com/me-no-dev/ESPAsyncWebServer
// # - ESPUI                  // by s00500:                       https://github.com/s00500/ESPUI
// # - ArduinoJson            // by bblanchon:                    https://github.com/bblanchon/ArduinoJson
// # - LITTLEFS               // by lorol:                        https://github.com/lorol/LITTLEFS
// #
// ###########################################################################################################################################
#include <WiFi.h>               // Used to connect the ESP32 to your WiFi
#include <WebServer.h>          // ESP32 OTA update function
#include <Update.h>             // ESP32 OTA update function
#include <Adafruit_NeoPixel.h>  // Used to drive the NeoPixel LEDs
#include "time.h"               // Used for NTP time requests
#include <AsyncTCP.h>           // Used for the internal web server
#include <ESPAsyncWebServer.h>  // Used for the internal web server
#include <DNSServer.h>          // Used for the internal web server
#include <ESPUI.h>              // Used for the internal web server
#include "esp_log.h"            // Disable WiFi debug warnings
#include <Preferences.h>        // Used to save the configuration to the ESP32 flash
#include "settings.h"           // Settings are stored in a seperate file to make to code better readable and to be able to switch to other settings faster


// ###########################################################################################################################################
// # Version number of the code:
// ###########################################################################################################################################
const char* CLOCK_VERSION = "V1.0.0";


// ###########################################################################################################################################
// # Internal web server settings:
// ###########################################################################################################################################
AsyncWebServer server(80);  // Web server for config
WebServer otaserver(8080);  // Web OTA ESP32 update server
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;


// ###########################################################################################################################################
// # Declartions and variables used in the functions:
// ###########################################################################################################################################
Preferences preferences;
int iHour = 0;
int iMinute = 0;
int iSecond = 0;
int iDay = 23;
int iMonth = 11;
int iYear = 2022;
bool updatedevice = true;
bool updatenow = false;
bool updatemode = false;
bool changedvalues = false;
String iStartTime = " ";
int redVal_back, greenVal_back, blueVal_back;
int redVal_time, greenVal_time, blueVal_time;
int intensity, intensity_day, intensity_night;
int usenightmode, day_time_start, day_time_stop, statusNightMode;
int statusLabelID, statusNightModeID, statusLanguageID, intensity_web_HintID, DayNightSectionID, LEDsettingsSectionID;
int sliderBrightnessDayID, switchNightModeID, sliderBrightnessNightID, call_day_time_startID, call_day_time_stopID;
uint16_t text_colour_background;
uint16_t text_colour_time;
bool WiFIsetup = false;


// ###########################################################################################################################################
// # Setup function that runs once at startup of the ESP:
// ###########################################################################################################################################
void setup() {
  Serial.begin(115200);
  delay(250);
  Serial.println("######################################################################");
  Serial.print("# Binary Matrix Clock startup of version: ");
  Serial.println(CLOCK_VERSION);
  Serial.println("######################################################################");
  preferences.begin("binaryclock", false);  // Init ESP32 flash
  getFlashValues();                         // Read settings from flash
  strip.begin();                            // Init the LEDs
  strip.show();                             // Init the LEDs --> Set them to OFF
  intensity = intensity_day;                // Set the intenity to day mode for startup
  strip.setBrightness(intensity);           // Set LED brightness
  WIFI_SETUP();                             // WiFi login and startup of web services
}


// ###########################################################################################################################################
// # Loop function which runs all the time after the startup was done:
// ###########################################################################################################################################
void loop() {
  if ((WiFIsetup == true) || (testTime == 1)) {
    printLocalTime();                               // Locally get the time (NTP server requests done 1x per hour)
    if (updatedevice == true) {                     // Allow display updates (normal usage)
      if (changedvalues == true) setFlashValues();  // Write settings to flash
      update_display();                             // Update display (1x per minute regulary)
    }
    if (updatemode == true) otaserver.handleClient();  // ESP32 OTA update
  }
  dnsServer.processNextRequest();  // Update the web server
}


// ###########################################################################################################################################
// # Setup the internal web server configuration page:
// ###########################################################################################################################################
void setupWebInterface() {
  dnsServer.start(DNS_PORT, "*", apIP);


  // Section General:
  // ################
  ESPUI.separator("General:");

  // Status label:
  statusLabelID = ESPUI.label("Status:", ControlColor::Dark, "Operational");

  // Binary Matrix Clock version:
  ESPUI.label("Version", ControlColor::None, CLOCK_VERSION);



  // Section LED settings:
  // #####################
  LEDsettingsSectionID = ESPUI.separator("LED settings:");

  // Time color selector:
  char hex_time[7] = { 0 };
  sprintf(hex_time, "#%02X%02X%02X", redVal_time, greenVal_time, blueVal_time);
  text_colour_time = ESPUI.text("Time", colCallTIME, ControlColor::Dark, hex_time);
  ESPUI.setInputType(text_colour_time, "color");

  // Background color selector:
  char hex_back[7] = { 0 };
  sprintf(hex_back, "#%02X%02X%02X", redVal_back, greenVal_back, blueVal_back);
  text_colour_background = ESPUI.text("Background", colCallBACK, ControlColor::Dark, hex_back);
  ESPUI.setInputType(text_colour_background, "color");



  // Section LED night mode settings:
  // ################################
  DayNightSectionID = ESPUI.separator("Day/Night LED brightness mode settings:");

  // Use night mode function:
  switchNightModeID = ESPUI.switcher("Use night mode to reduce brightness", &switchNightMode, ControlColor::Dark, usenightmode);

  // Intensity DAY slider selector: !!! DEFAULT LIMITED TO 64 of 255 !!!
  sliderBrightnessDayID = ESPUI.slider("Brightness during the day", &sliderBrightnessDay, ControlColor::Dark, intensity_day, 0, LEDintensityLIMIT);

  // Intensity NIGHT slider selector: !!! DEFAULT LIMITED TO 64 of 255 !!!
  sliderBrightnessNightID = ESPUI.slider("Brightness at night", &sliderBrightnessNight, ControlColor::Dark, intensity_night, 0, LEDintensityLIMIT);

  // Night mode status:
  statusNightModeID = ESPUI.label("Night mode status", ControlColor::Dark, "Night mode not used");

  // Day mode start time:
  call_day_time_startID = ESPUI.number("Day time starts at", call_day_time_start, ControlColor::Dark, day_time_start, 0, 11);

  // Day mode stop time:
  call_day_time_stopID = ESPUI.number("Day time ends after", call_day_time_stop, ControlColor::Dark, day_time_stop, 12, 23);



  // Section WiFi:
  // #############
  ESPUI.separator("WiFi:");

  // WiFi SSID:
  ESPUI.label("SSID", ControlColor::Dark, WiFi.SSID());

  // WiFi signal strength:
  ESPUI.label("Signal", ControlColor::Dark, String(WiFi.RSSI()) + "dBm");

  // Hostname:
  ESPUI.label("Hostname", ControlColor::Dark, hostname);

  // WiFi ip-address:
  ESPUI.label("IP-address", ControlColor::Dark, IpAddress2String(WiFi.localIP()));

  // WiFi MAC-address:
  ESPUI.label("MAC address", ControlColor::Dark, WiFi.macAddress());



  // Section Time settings:
  // ######################
  ESPUI.separator("Time settings:");

  // NTP server:
  ESPUI.label("NTP server", ControlColor::Dark, NTPserver);

  // Time zone:
  ESPUI.label("Time zone", ControlColor::Dark, Timezone);

  // Time:
  ESPUI.label("Startup time", ControlColor::Dark, iStartTime);



  // Section Update:
  // ###############
  ESPUI.separator("Update:");

  // Update Binary Matrix Clock:
  ESPUI.button("Activate update mode", &buttonUpdate, ControlColor::Dark, "Activate update mode", (void*)1);

  // Update URL
  ESPUI.label("Update URL", ControlColor::Dark, "http://" + IpAddress2String(WiFi.localIP()) + ":8080");

  // AWSW software GitHub repository:
  ESPUI.label("Download newer software updates here", ControlColor::Dark, "https://github.com/AWSW-de/Binary-Matrix-Clock");



  // Section Maintenance:
  // ####################
  ESPUI.separator("Maintenance:");

  // Restart:
  ESPUI.button("Restart", &buttonRestart, ControlColor::Dark, "Restart", (void*)1);

  // Reset WiFi settings:
  ESPUI.button("Reset WiFi settings", &buttonWiFiReset, ControlColor::Dark, "Reset WiFi settings", (void*)2);

  // Reset settings:
  ESPUI.button("Reset settings", &buttonClockReset, ControlColor::Dark, "Reset settings", (void*)3);



  // Section License:
  // ####################
  ESPUI.separator("License information:");

  // License information:
  ESPUI.label("License information", ControlColor::Dark, "NonCommercial — You may not use the project for commercial purposes!");



  // Update night mode status text on startup:
  if (usenightmode == 1) {
    if ((iHour >= day_time_start) && (iHour <= day_time_stop)) {
      ESPUI.print(statusNightModeID, "Day time");
      if ((iHour == 0) && (day_time_stop == 23)) ESPUI.print(statusNightModeID, "Night time");  // Special function if day_time_stop set to 23 and time is 24, so 0...
    } else {
      ESPUI.print(statusNightModeID, "Night time");
    }
  }

  // Deploy the page:
  ESPUI.begin("Binary Matrix Clock");
}


// ###########################################################################################################################################
// # Read settings from flash:
// ###########################################################################################################################################
void getFlashValues() {
  if (debugtexts == 1) Serial.println("Read settings from flash: START");
  redVal_time = preferences.getUInt("redVal_time", redVal_time_default);
  greenVal_time = preferences.getUInt("greenVal_time", greenVal_time_default);
  blueVal_time = preferences.getUInt("blueVal_time", blueVal_time_default);
  redVal_back = preferences.getUInt("redVal_back", redVal_back_default);
  greenVal_back = preferences.getUInt("greenVal_back", greenVal_back_default);
  blueVal_back = preferences.getUInt("blueVal_back", blueVal_back_default);
  intensity_day = preferences.getUInt("intensity_day", intensity_day_default);
  intensity_night = preferences.getUInt("intensity_night", intensity_night_default);
  usenightmode = preferences.getUInt("usenightmode", usenightmode_default);
  day_time_start = preferences.getUInt("day_time_start", day_time_start_default);
  day_time_stop = preferences.getUInt("day_time_stop", day_time_stop_default);
  if (debugtexts == 1) Serial.println("Read settings from flash: END");
}


// ###########################################################################################################################################
// # Write settings to flash:
// ###########################################################################################################################################
void setFlashValues() {
  if (debugtexts == 1) Serial.println("Write settings to flash: START");
  changedvalues = false;
  preferences.putUInt("redVal_time", redVal_time);
  preferences.putUInt("greenVal_time", greenVal_time);
  preferences.putUInt("blueVal_time", blueVal_time);
  preferences.putUInt("redVal_back", redVal_back);
  preferences.putUInt("greenVal_back", greenVal_back);
  preferences.putUInt("blueVal_back", blueVal_back);
  preferences.putUInt("intensity_day", intensity_day);
  preferences.putUInt("intensity_night", intensity_night);
  preferences.putUInt("usenightmode", usenightmode);
  preferences.putUInt("day_time_start", day_time_start);
  preferences.putUInt("day_time_stop", day_time_stop);
  if (debugtexts == 1) Serial.println("Write settings to flash: END");
  if (usenightmode == 1) {
    if ((iHour >= day_time_start) && (iHour <= day_time_stop)) {
      ESPUI.print(statusNightModeID, "Day time");
      if ((iHour == 0) && (day_time_stop == 23)) ESPUI.print(statusNightModeID, "Night time");  // Special function if day_time_stop set to 23 and time is 24, so 0...
    } else {
      ESPUI.print(statusNightModeID, "Night time");
    }
  } else {
    ESPUI.print(statusNightModeID, "Night mode not used");
  }
  updatenow = true;  // Update display now...
}


// ###########################################################################################################################################
// # GUI: Reset the settings:
// ###########################################################################################################################################
int ClockResetCounter = 0;
void buttonClockReset(Control* sender, int type, void* param) {
  updatedevice = false;
  delay(100);
  switch (type) {
    case B_DOWN:
      break;
    case B_UP:
      if (ClockResetCounter == 1) {
        Serial.println("Status: SETTINGS RESET REQUEST EXECUTED");
        // Save stored values for WiFi:
        String tempDelWiFiSSID = preferences.getString("WIFIssid");
        String tempDelWiFiPASS = preferences.getString("WIFIpass");
        preferences.clear();
        delay(100);
        preferences.putString("WIFIssid", tempDelWiFiSSID);  // Restore entered WiFi SSID
        preferences.putString("WIFIpass", tempDelWiFiPASS);  // Restore entered WiFi password
        preferences.putUInt("redVal_time", redVal_time_default);
        preferences.putUInt("greenVal_time", greenVal_time_default);
        preferences.putUInt("blueVal_time", blueVal_time_default);
        preferences.putUInt("redVal_back", redVal_back_default);
        preferences.putUInt("greenVal_back", greenVal_back_default);
        preferences.putUInt("blueVal_back", blueVal_back_default);
        preferences.putUInt("intensity_day", intensity_day_default);
        preferences.putUInt("intensity_night", intensity_night_default);
        preferences.putUInt("usenightmode", usenightmode_default);
        preferences.putUInt("day_time_stop", day_time_stop_default);
        preferences.putUInt("day_time_stop", day_time_stop_default);
        delay(100);
        preferences.end();
        Serial.println("######################################################################################");
        Serial.println("# SETTING WERE SET TO DEFAULT... CLOCK WILL NOW RESTART... PLEASE CONFIGURE AGAIN... #");
        Serial.println("######################################################################################");
        ClearDisplay();
        strip.show();
        delay(250);
        ESP.restart();
      } else {
        Serial.println("Status: SETTINGS RESET REQUEST");
        ESPUI.print(statusLabelID, "SETTINGS RESET REQUESTED");
        ESPUI.updateButton(sender->id, "! Press button once more to apply settings reset !");
        ClockResetCounter = ClockResetCounter + 1;
      }
      break;
  }
}


// ###########################################################################################################################################
// # Clear the display:
// ###########################################################################################################################################
void ClearDisplay() {
  uint32_t c0 = strip.Color(0, 0, 0);
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, c0);
  }
}


// ###########################################################################################################################################
// # GUI: Restart the clock:
// ###########################################################################################################################################
void buttonRestart(Control* sender, int type, void* param) {
  updatedevice = false;
  if (changedvalues == true) setFlashValues();  // Write settings to flash
  preferences.end();
  delay(250);
  ESP.restart();
}


// ###########################################################################################################################################
// # GUI: Reset the WiFi settings of the clock:
// ###########################################################################################################################################
void buttonWiFiReset(Control* sender, int type, void* param) {
  updatedevice = false;
  Serial.println("Status: WIFI SETTINGS RESET REQUEST");
  WiFi.disconnect();  // DISCONNECT FROM WIFI
  delay(1000);
  preferences.putString("WIFIssid", "");  // Reset WiFi SSID
  preferences.putString("WIFIpass", "");  // Reste WiFi password
  preferences.end();
  Serial.println("Status: WIFI SETTINGS RESET REQUEST EXECUTED");
  Serial.println("################################################################################################");
  Serial.println("# WIFI SETTING WERE SET TO DEFAULT... CLOCK WILL NOW RESTART... PLEASE CONFIGURE WIFI AGAIN... #");
  Serial.println("################################################################################################");
  delay(500);
  ESP.restart();
}


// ###########################################################################################################################################
// # GUI: Update the clock:
// ###########################################################################################################################################
void buttonUpdate(Control* sender, int type, void* param) {
  preferences.end();
  updatedevice = false;
  ESPUI.print(statusLabelID, "Update requested");
  ESPUI.updateButton(sender->id, "Update mode active now - Use the update url: >>>");
  if (updatemode == false) {
    updatemode = true;
    Serial.println("Status: Update request");
  }
}


// ###########################################################################################################################################
// # Actual function, which controls 1/0 of the LED with color value:
// ###########################################################################################################################################
void setLEDcol(int ledNrFrom, int ledNrTo, uint32_t color) {
  if (ledNrFrom > ledNrTo) {
    setLED(ledNrTo, ledNrFrom, 1);  // Sets LED numbers in correct order
  } else {
    for (int i = ledNrFrom; i <= ledNrTo; i++) {
      if ((i >= 0) && (i < NUMPIXELS))
        strip.setPixelColor(i, color);
    }
  }
}


// ###########################################################################################################################################
// # GUI: Night mode switch:
// ###########################################################################################################################################
void switchNightMode(Control* sender, int value) {
  updatedevice = false;
  delay(1000);
  switch (value) {
    case S_ACTIVE:
      usenightmode = 1;
      if ((iHour >= day_time_start) && (iHour <= day_time_stop)) {
        intensity = intensity_day;
        if ((iHour == 0) && (day_time_stop == 23)) intensity = intensity_night;  // Special function if day_time_stop set to 23 and time is 24, so 0...
      } else {
        intensity = intensity_night;
      }
      break;
    case S_INACTIVE:
      intensity = intensity_day;
      usenightmode = 0;
      break;
  }
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # Update the display / time on it:
// ###########################################################################################################################################
void update_display() {
  // Show the current time or use the time text test function:
  if (testTime == 0) {  // Show the current time:
    show_time(iHour, iMinute, iSecond);
  } else {  // TEST THE DISPLAY TIME OUTPUT:
    ClearDisplay();
    strip.show();
    Serial.println(" ");
    Serial.println("Show 'TEST' text...");
    strip.setBrightness(25);
    redVal_back = 0;
    greenVal_back = 0;
    blueVal_back = 0;
    usenightmode = 0;
    uint32_t c = strip.Color(redVal_time, greenVal_time, blueVal_time);
    for (int i = 0; i <= 59; i++) {
      show_time(i, i, i);
      delay(5000);
    }
  }
}


// ###########################################################################################################################################
// # Display hours and minutes text function:
// ###########################################################################################################################################
uint32_t colorRGB;
static int lastHourSet = -1;
static int lastMinutesSet = -1;
static int lastSecondsSet = -1;
void show_time(int hours, int minutes, int seconds) {

  if ((lastHourSet == hours && lastMinutesSet == minutes && lastSecondsSet == seconds) && updatenow == false) {  // Reduce display updates to new seconds and new config updates
    return;
  }

  updatenow = false;
  lastHourSet = hours;
  lastMinutesSet = minutes;
  lastSecondsSet = seconds;

  // Show current time of display update:
  if (debugtexts == 1) Serial.println("Update display now: " + String(hours) + ":" + String(minutes) + ":" + String(seconds));

  // Night/Day mode intensity setting:
  if (usenightmode == 1) {
    if ((iHour >= day_time_start) && (iHour <= day_time_stop)) {
      intensity = intensity_day;
      if ((iHour == 0) && (day_time_stop == 23)) intensity = intensity_night;  // Special function if day_time_stop set to 23 and time is 24, so 0...
    } else {
      intensity = intensity_night;
    }
    if (testDayNightmode == 1) {  // Test day/night times function:
      Serial.println("############################################################################################");
      Serial.println("Current time day/night test: " + String(hours) + ":" + String(minutes) + ":" + String(iSecond));
      Serial.println("Current settings: day_time_start: " + String(day_time_start) + " day_time_stop: " + String(day_time_stop));
      for (int i = 0; i < 24; i++) {
        String daynightvar = "-";
        if ((i >= day_time_start) && (i <= day_time_stop)) {
          daynightvar = "Day time";
          if ((i == 0) && (day_time_stop == 23)) daynightvar = "Night time";
        } else {
          daynightvar = "Night time";
        }
        Serial.println("Current hour: " + String(i) + " --> " + daynightvar);
      }
      testDayNightmode = 0;  // Show the list 1x only
      Serial.println("############################################################################################");
    }
  } else {  // Control intensity by Binary Matrix Clock settings or via HTML command:
    intensity = intensity_day;
  }
  strip.setBrightness(intensity);

  // Set background color:
  back_color();

  // Set the time text color:
  colorRGB = strip.Color(redVal_time, greenVal_time, blueVal_time);

  // Display time:
  iHour = hours;
  iMinute = minutes;
  iSecond = seconds;

  // Test a special time:
  if (testspecialtime == 1) {
    Serial.println("Special time test active: " + String(test_hour) + ":" + String(test_minute) + ":" + String(test_second));
    iHour = test_hour;
    iMinute = test_minute;
    iSecond = test_second;
  }

  // ########################################################### GENERAL 1st LINE

  // 1st line (32,16,8,4,2,1):
  setLEDcol(18, 29, colorRGB);
  setLEDcol(34, 45, colorRGB);  // 2nd row

  // ########################################################### HOURS: (1-23)

  // 1: (hours)
  if (iHour <= 23) {  // Avoid invalid test function values
    if ((iHour % 10 == 1) || (iHour % 10 == 3) || (iHour % 10 == 5) || (iHour % 10 == 7) || (iHour % 10 == 9)) {
      setLEDcol(92, 93, colorRGB);
      setLEDcol(98, 99, colorRGB);  // 2nd row
    }
    // 2: (hours):
    if ((iHour == 2) || (iHour == 3) || (iHour == 6) || (iHour == 7) || (iHour == 10) || (iHour == 11) || (iHour == 14) || (iHour == 15) || (iHour == 18) || (iHour == 19) || (iHour == 22) || (iHour == 23)) {
      setLEDcol(90, 91, colorRGB);
      setLEDcol(100, 101, colorRGB);  // 2nd row
    }
    // 4: (hours)
    if ((iHour == 4) || (iHour == 5) || (iHour == 6) || (iHour == 7) || (iHour == 12) || (iHour == 13) || (iHour == 14) || (iHour == 15) || (iHour == 20) || (iHour == 21) || (iHour == 22) || (iHour == 23)) {
      setLEDcol(88, 89, colorRGB);
      setLEDcol(102, 103, colorRGB);  // 2nd row
    }
    // 8: (hours)
    if ((iHour == 8) || (iHour == 9) || (iHour == 10) || (iHour == 11) || (iHour == 12) || (iHour == 13) || (iHour == 14) || (iHour == 15)) {
      setLEDcol(86, 87, colorRGB);
      setLEDcol(104, 105, colorRGB);  // 2nd row
    }
    // 16: (hours)
    if ((iHour == 16) || (iHour == 17) || (iHour == 18) || (iHour == 19) || (iHour == 20) || (iHour == 21) || (iHour == 22) || (iHour == 23)) {
      setLEDcol(84, 85, colorRGB);
      setLEDcol(106, 107, colorRGB);  // 2nd row
    }
  }

  // ########################################################### MINUTES: (0-59)

  // 1: (Minutes)
  if ((iMinute % 10 == 1) || (iMinute % 10 == 3) || (iMinute % 10 == 5) || (iMinute % 10 == 7) || (iMinute % 10 == 9)) {
    setLEDcol(156, 157, colorRGB);
    setLEDcol(162, 163, colorRGB);  // 2nd row
  }
  // 2: (Minutes):
  if ((iMinute == 2) || (iMinute == 3) || (iMinute == 6) || (iMinute == 7) || (iMinute == 10) || (iMinute == 11) || (iMinute == 14) || (iMinute == 15) || (iMinute == 18) || (iMinute == 19) || (iMinute == 22) || (iMinute == 23) || (iMinute == 26) || (iMinute == 27) || (iMinute == 30) || (iMinute == 31) || (iMinute == 34) || (iMinute == 35) || (iMinute == 38) || (iMinute == 39) || (iMinute == 42) || (iMinute == 43) || (iMinute == 46) || (iMinute == 47) || (iMinute == 50) || (iMinute == 51) || (iMinute == 54) || (iMinute == 55) || (iMinute == 58) || (iMinute == 59)) {
    setLEDcol(154, 155, colorRGB);
    setLEDcol(164, 165, colorRGB);  // 2nd row
  }
  // 4: (Minutes)
  if ((iMinute == 4) || (iMinute == 5) || (iMinute == 6) || (iMinute == 7) || (iMinute == 12) || (iMinute == 13) || (iMinute == 14) || (iMinute == 15) || (iMinute == 20) || (iMinute == 21) || (iMinute == 22) || (iMinute == 23) || (iMinute == 28) || (iMinute == 29) || (iMinute == 30) || (iMinute == 31) || (iMinute == 36) || (iMinute == 37) || (iMinute == 38) || (iMinute == 39) || (iMinute == 44) || (iMinute == 45) || (iMinute == 46) || (iMinute == 47) || (iMinute == 52) || (iMinute == 53) || (iMinute == 54) || (iMinute == 55)) {
    setLEDcol(152, 153, colorRGB);
    setLEDcol(166, 167, colorRGB);  // 2nd row
  }
  // 8: (Minutes)
  if ((iMinute == 8) || (iMinute == 9) || (iMinute == 10) || (iMinute == 11) || (iMinute == 12) || (iMinute == 13) || (iMinute == 14) || (iMinute == 15) || (iMinute == 24) || (iMinute == 25) || (iMinute == 26) || (iMinute == 27) || (iMinute == 28) || (iMinute == 29) || (iMinute == 30) || (iMinute == 31) || (iMinute == 40) || (iMinute == 41) || (iMinute == 42) || (iMinute == 43) || (iMinute == 44) || (iMinute == 45) || (iMinute == 46) || (iMinute == 47) || (iMinute == 56) || (iMinute == 57) || (iMinute == 58) || (iMinute == 59)) {
    setLEDcol(150, 151, colorRGB);
    setLEDcol(168, 169, colorRGB);  // 2nd row
  }
  // 16: (Minutes)
  if ((iMinute == 16) || (iMinute == 17) || (iMinute == 18) || (iMinute == 19) || (iMinute == 20) || (iMinute == 21) || (iMinute == 22) || (iMinute == 23) || (iMinute == 24) || (iMinute == 25) || (iMinute == 26) || (iMinute == 27) || (iMinute == 28) || (iMinute == 29) || (iMinute == 30) || (iMinute == 31) || (iMinute == 48) || (iMinute == 49) || (iMinute == 50) || (iMinute == 51) || (iMinute == 52) || (iMinute == 53) || (iMinute == 54) || (iMinute == 55) || (iMinute == 56) || (iMinute == 57) || (iMinute == 58) || (iMinute == 59)) {
    setLEDcol(148, 149, colorRGB);
    setLEDcol(170, 171, colorRGB);  // 2nd row
  }
  // 32: (Minutes)
  if (iMinute >= 32) {
    setLEDcol(146, 147, colorRGB);
    setLEDcol(172, 173, colorRGB);  // 2nd row
  }

  // ########################################################### SECONDS: (0-59)

  // 1: (Seconds)
  if ((iSecond % 10 == 1) || (iSecond % 10 == 3) || (iSecond % 10 == 5) || (iSecond % 10 == 7) || (iSecond % 10 == 9)) {
    setLEDcol(220, 221, colorRGB);
    setLEDcol(226, 227, colorRGB);  // 2nd row
  }
  // 2: (Seconds):
  if ((iSecond == 2) || (iSecond == 3) || (iSecond == 6) || (iSecond == 7) || (iSecond == 10) || (iSecond == 11) || (iSecond == 14) || (iSecond == 15) || (iSecond == 18) || (iSecond == 19) || (iSecond == 22) || (iSecond == 23) || (iSecond == 26) || (iSecond == 27) || (iSecond == 30) || (iSecond == 31) || (iSecond == 34) || (iSecond == 35) || (iSecond == 38) || (iSecond == 39) || (iSecond == 42) || (iSecond == 43) || (iSecond == 46) || (iSecond == 47) || (iSecond == 50) || (iSecond == 51) || (iSecond == 54) || (iSecond == 55) || (iSecond == 58) || (iSecond == 59)) {
    setLEDcol(218, 219, colorRGB);
    setLEDcol(228, 229, colorRGB);  // 2nd row
  }
  // 4: (Seconds)
  if ((iSecond == 4) || (iSecond == 5) || (iSecond == 6) || (iSecond == 7) || (iSecond == 12) || (iSecond == 13) || (iSecond == 14) || (iSecond == 15) || (iSecond == 20) || (iSecond == 21) || (iSecond == 22) || (iSecond == 23) || (iSecond == 28) || (iSecond == 29) || (iSecond == 30) || (iSecond == 31) || (iSecond == 36) || (iSecond == 37) || (iSecond == 38) || (iSecond == 39) || (iSecond == 44) || (iSecond == 45) || (iSecond == 46) || (iSecond == 47) || (iSecond == 52) || (iSecond == 53) || (iSecond == 54) || (iSecond == 55)) {
    setLEDcol(216, 217, colorRGB);
    setLEDcol(230, 231, colorRGB);  // 2nd row
  }
  // 8: (Seconds)
  if ((iSecond == 8) || (iSecond == 9) || (iSecond == 10) || (iSecond == 11) || (iSecond == 12) || (iSecond == 13) || (iSecond == 14) || (iSecond == 15) || (iSecond == 24) || (iSecond == 25) || (iSecond == 26) || (iSecond == 27) || (iSecond == 28) || (iSecond == 29) || (iSecond == 30) || (iSecond == 31) || (iSecond == 40) || (iSecond == 41) || (iSecond == 42) || (iSecond == 43) || (iSecond == 44) || (iSecond == 45) || (iSecond == 46) || (iSecond == 47) || (iSecond == 56) || (iSecond == 57) || (iSecond == 58) || (iSecond == 59)) {
    setLEDcol(214, 215, colorRGB);
    setLEDcol(232, 233, colorRGB);  // 2nd row
  }
  // 16: (Seconds)
  if ((iSecond == 16) || (iSecond == 17) || (iSecond == 18) || (iSecond == 19) || (iSecond == 20) || (iSecond == 21) || (iSecond == 22) || (iSecond == 23) || (iSecond == 24) || (iSecond == 25) || (iSecond == 26) || (iSecond == 27) || (iSecond == 28) || (iSecond == 29) || (iSecond == 30) || (iSecond == 31) || (iSecond == 48) || (iSecond == 49) || (iSecond == 50) || (iSecond == 51) || (iSecond == 52) || (iSecond == 53) || (iSecond == 54) || (iSecond == 55) || (iSecond == 56) || (iSecond == 57) || (iSecond == 58) || (iSecond == 59)) {
    setLEDcol(212, 213, colorRGB);
    setLEDcol(234, 235, colorRGB);  // 2nd row
  }
  // 32: (Seconds)
  if (iSecond >= 32) {
    setLEDcol(210, 211, colorRGB);
    setLEDcol(236, 237, colorRGB);  // 2nd row
  }

  strip.show();
}





// ###########################################################################################################################################
// # Background color function: SET ALL LEDs OFF
// ###########################################################################################################################################
void back_color() {
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));  // Clear all pixels
  }
  uint32_t c0 = strip.Color(redVal_back, greenVal_back, blueVal_back);
  for (int i = 82; i <= 93; i++) {  // Background color line hours
    strip.setPixelColor(i, c0);
  }
  for (int i = 98; i <= 109; i++) {
    strip.setPixelColor(i, c0);
  }
  for (int i = 146; i <= 157; i++) {  // Background color line minutes
    strip.setPixelColor(i, c0);
  }
  for (int i = 162; i <= 173; i++) {
    strip.setPixelColor(i, c0);
  }
  for (int i = 210; i <= 221; i++) {  // Background color line seconds
    strip.setPixelColor(i, c0);
  }
  for (int i = 226; i <= 237; i++) {
    strip.setPixelColor(i, c0);
  }
}


// ###########################################################################################################################################
// # Actual function, which controls 1/0 of the LED:
// ###########################################################################################################################################
void setLED(int ledNrFrom, int ledNrTo, int switchOn) {
  if (ledNrFrom > ledNrTo) {
    setLED(ledNrTo, ledNrFrom, switchOn);  // Sets LED numbers in correct order
  } else {
    for (int i = ledNrFrom; i <= ledNrTo; i++) {
      if ((i >= 0) && (i < NUMPIXELS))
        strip.setPixelColor(i, strip.Color(redVal_time, greenVal_time, blueVal_time));
    }
  }
  if (switchOn == 0) {
    for (int i = ledNrFrom; i <= ledNrTo; i++) {
      if ((i >= 0) && (i < NUMPIXELS))
        strip.setPixelColor(i, strip.Color(0, 0, 0));  // Switch LEDs off
    }
  }
}


// ###########################################################################################################################################
// # NTP time functions:
// ###########################################################################################################################################
void configNTPTime() {
  initTime(Timezone);
  printLocalTime();
}
// ###########################################################################################################################################
void setTimezone(String timezone) {
  Serial.printf("Setting timezone to %s\n", timezone.c_str());
  setenv("TZ", timezone.c_str(), 1);  //  Now adjust the TZ.  Clock settings are adjusted to show the new local time
  tzset();
}
// ###########################################################################################################################################
int TimeResetCounter = 1;
void initTime(String timezone) {
  struct tm timeinfo;
  Serial.println("Setting up time");
  configTime(0, 0, NTPserver);
  while (!getLocalTime(&timeinfo)) {
    delay(250);
    Serial.println("! Failed to obtain time - Time server could not be reached ! --> Try: " + String(TimeResetCounter) + " of 3...");
    TimeResetCounter = TimeResetCounter + 1;
    if (TimeResetCounter == 4) {
      Serial.println("! Failed to obtain time - Time server could not be reached ! --> RESTART THE DEVICE NOW...");
      ESP.restart();
    }
  }
  // Time successfully received:
  delay(1000);
  Serial.println("Got the time from NTP");
  setTimezone(timezone);
}
// ###########################################################################################################################################
void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return;
  }
  // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S zone %Z %z ");
  char timeStringBuff[50];
  strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);
  iStartTime = String(timeStringBuff);
  iHour = timeinfo.tm_hour;
  iMinute = timeinfo.tm_min;
  iSecond = timeinfo.tm_sec;
  // Serial.print("Time: ");
  // Serial.print(iHour);
  // Serial.print(":");
  // Serial.print(iMinute);
  // Serial.print(":");
  // Serial.println(iSecond);
  delay(1000);
}
// ###########################################################################################################################################
void setTime(int yr, int month, int mday, int hr, int minute, int sec, int isDst) {
  struct tm tm;
  tm.tm_year = yr - 1900;  // Set date
  tm.tm_mon = month - 1;
  tm.tm_mday = mday;
  tm.tm_hour = hr;  // Set time
  tm.tm_min = minute;
  tm.tm_sec = sec;
  tm.tm_isdst = isDst;  // 1 or 0
  time_t t = mktime(&tm);
  Serial.printf("Setting time: %s", asctime(&tm));
  struct timeval now = { .tv_sec = t };
  settimeofday(&now, NULL);
}
// ###########################################################################################################################################


// ###########################################################################################################################################
// # GUI: Convert hex color value to RGB int values - TIME:
// ###########################################################################################################################################
void getRGBTIME(String hexvalue) {
  updatedevice = false;
  delay(1000);
  hexvalue.toUpperCase();
  char c[7];
  hexvalue.toCharArray(c, 8);
  int red = hexcolorToInt(c[1], c[2]);
  int green = hexcolorToInt(c[3], c[4]);
  int blue = hexcolorToInt(c[5], c[6]);
  redVal_time = red;
  greenVal_time = green;
  blueVal_time = blue;
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Convert hex color value to RGB int values - BACKGROUND:
// ###########################################################################################################################################
void getRGBBACK(String hexvalue) {
  updatedevice = false;
  delay(1000);
  hexvalue.toUpperCase();
  char c[7];
  hexvalue.toCharArray(c, 8);
  int red = hexcolorToInt(c[1], c[2]);
  int green = hexcolorToInt(c[3], c[4]);
  int blue = hexcolorToInt(c[5], c[6]);
  redVal_back = red;
  greenVal_back = green;
  blueVal_back = blue;
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Convert hex color value to RGB int values - helper function:
// ###########################################################################################################################################
int hexcolorToInt(char upper, char lower) {
  int uVal = (int)upper;
  int lVal = (int)lower;
  uVal = uVal > 64 ? uVal - 55 : uVal - 48;
  uVal = uVal << 4;
  lVal = lVal > 64 ? lVal - 55 : lVal - 48;
  return uVal + lVal;
}


// ###########################################################################################################################################
// # GUI: Color change for time color:
// ###########################################################################################################################################
void colCallTIME(Control* sender, int type) {
  getRGBTIME(sender->value);
}


// ###########################################################################################################################################
// # GUI: Color change for background color:
// ###########################################################################################################################################
void colCallBACK(Control* sender, int type) {
  getRGBBACK(sender->value);
}


// ###########################################################################################################################################
// # GUI: Slider change for LED intensity: DAY
// ###########################################################################################################################################
void sliderBrightnessDay(Control* sender, int type) {
  updatedevice = false;
  delay(1000);
  intensity_day = sender->value.toInt();
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Slider change for LED intensity: NIGHT
// ###########################################################################################################################################
void sliderBrightnessNight(Control* sender, int type) {
  updatedevice = false;
  delay(1000);
  intensity_night = sender->value.toInt();
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Time Day Mode Start
// ###########################################################################################################################################
void call_day_time_start(Control* sender, int type) {
  updatedevice = false;
  delay(1000);
  day_time_start = sender->value.toInt();
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Time Day Mode Stop
// ###########################################################################################################################################
void call_day_time_stop(Control* sender, int type) {
  updatedevice = false;
  delay(1000);
  day_time_stop = sender->value.toInt();
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Convert IP-address value to string:
// ###########################################################################################################################################
String IpAddress2String(const IPAddress& ipAddress) {
  return String(ipAddress[0]) + String(".") + String(ipAddress[1]) + String(".") + String(ipAddress[2]) + String(".") + String(ipAddress[3]);
}


// ###########################################################################################################################################
// # Wifi scan function to help you to setup your WiFi connection
// ###########################################################################################################################################
void ScanWiFi() {
  Serial.println("Scan WiFi networks - START");
  int n = WiFi.scanNetworks();
  Serial.println("WiFi scan done");
  Serial.println(" ");
  if (n == 0) {
    Serial.println("No WiFi networks found");
  } else {
    Serial.print(n);
    Serial.println(" WiFi networks found:");
    Serial.println(" ");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("Scan WiFi networks - END");
}


// ###########################################################################################################################################
// # Captive Portal web page to setup the device by AWSW:
// ###########################################################################################################################################
const char index_html[] PROGMEM = R"=====(
  <!DOCTYPE html><html><head><title>Binary Matrix Clock</title></head>
          <style>
      body {
      padding: 25px;
      font-size: 25px;
      background-color: black;
      color: white;
      }
      </style>
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
  
   <style>
    .button {
      display: inline-block;
      padding: 15px 25px;
      font-size: 24px;
      cursor: pointer;
      text-align: center;
      text-decoration: none;
      outline: none;
      color: #fff;
      background-color: #4CAF50;
      border: none;
      border-radius: 15px;
      box-shadow: 0 9px #999;
    }
    .button:hover {background-color: #3e8e41}
    .button:active {
      background-color: #3e8e41;
      box-shadow: 0 5px #666;
      transform: translateY(4px);
    }
    </style>
  
  <body>
    <form action="/start" name="myForm">
      <center><b><h1>Welcome to the Binary Matrix Clock setup</h1></b>
      <h2>Please add your local WiFi credentials<br/><br/>on the next page</h2><br/>
      <input type="submit" value="Configure Binary Matrix Clock" class="button">
     </center></form></body>
  </html>
 )=====";


// ###########################################################################################################################################
// # Captive Portal web page to setup the device by AWSW:
// ###########################################################################################################################################
const char config_html[] PROGMEM = R"rawliteral(
 <!DOCTYPE HTML><html><head><title>Binary Matrix Clock</title>
 <meta name="viewport" content="width=device-width, initial-scale=1">
  <script language="JavaScript">
  <!--
  function validateForm() {
  var x = document.forms["myForm"]["mySSID"].value;
  if (x == "") {
    alert("WiFi SSID must be set");
    return false;
  }
  var y = document.forms["myForm"]["myPW"].value;
  if (y == "") {
    alert("WiFi password must be set");
    return false;
  }
  } 
  //-->
  </script>
  </head>
  
   <style>
      body {
      padding: 25px;
      font-size: 25px;
      background-color: black;
      color: white;
      }
      </style>
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
  
   <style>
    .button {
      display: inline-block;
      padding: 15px 25px;
      font-size: 24px;
      cursor: pointer;
      text-align: center;
      text-decoration: none;
      outline: none;
      color: #fff;
      background-color: #4CAF50;
      border: none;
      border-radius: 15px;
      box-shadow: 0 9px #999;
    }
    .button:hover {background-color: #3e8e41}
    .button:active {
      background-color: #3e8e41;
      box-shadow: 0 5px #666;
      transform: translateY(4px);
    }
    </style>
  
  <body>
  <form action="/get" name="myForm" onsubmit="return validateForm()" >
    <center><b><h1>Initial Binary Matrix Clock setup:</h1></b>
    <label for="mySSID">Enter your WiFi SSID:</label><br/>
    <input type="text" id="mySSID" name="mySSID" value="" style="width: 200px;" /><br/><br/>
    <label for="myPW">Enter your WiFi password:</label><br/>
    <input type="text" id="myPW" name="myPW" value="" style="width: 200px;" /><br/><br/>
    <input type="submit" value="Save values and start Binary Matrix Clock" class="button">
  </center></form></body></html>)rawliteral";


// ###########################################################################################################################################
// # Captive Portal web page to setup the device by AWSW:
// ###########################################################################################################################################
const char saved_html[] PROGMEM = R"rawliteral(
 <!DOCTYPE HTML><html><head>
  <title>Initial Binary Matrix Clock setup</title>
  <meta name="viewport" content="width=device-width, initial-scale=1"></head>
    <style>
  body {
      padding: 25px;
      font-size: 25px;
      background-color: black;
      color: white;
    }
  </style>
  <body>
    <center><h2><b>Settings saved...<br><br>
    Binary Matrix Clock will now try to connect to the named WiFi.<br>
    If it failes please try to connect to the temporary access point again.<br>
    Please close this page now and enjoy your Binary Matrix Clock. =)</h2></b>
 </body></html>)rawliteral";


// ###########################################################################################################################################
// # Captive Portal by AWSW to avoid the usage of the WiFi Manager library to have more control
// ###########################################################################################################################################
const char* PARAM_INPUT_1 = "mySSID";
const char* PARAM_INPUT_2 = "myPW";
const String captiveportalURL = "http://192.168.4.1";
void CaptivePotalSetup() {
  ScanWiFi();
  const char* temp_ssid = "Binary Matrix Clock";
  const char* temp_password = "";
  WiFi.softAP(temp_ssid, temp_password);
  Serial.println(" ");
  Serial.println(" ");
  Serial.println(" ");
  Serial.println("#################################################################################################################################################################################");
  Serial.print("# Temporary WiFi access point initialized. Please connect to the WiFi access point now and set your local WiFi credentials and Binary Matrix Clock language. Access point name: ");
  Serial.println(temp_ssid);
  Serial.print("# In case your browser does not open the Binary Matrix Clock setup page automatically after connecting to the access point, please navigate to this URL manually to http://");
  Serial.println(WiFi.softAPIP());
  Serial.println("#################################################################################################################################################################################");
  Serial.println(" ");
  Serial.println(" ");
  Serial.println(" ");
  dnsServer.start(53, "*", WiFi.softAPIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/html", index_html);
  });

  server.on("/get", HTTP_GET, [](AsyncWebServerRequest* request) {
    String inputMessage;
    String inputParam;
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      // Serial.println(inputMessage);
      preferences.putString("WIFIssid", inputMessage);  // Save entered WiFi SSID
      inputMessage = request->getParam(PARAM_INPUT_2)->value();
      inputParam = PARAM_INPUT_2;
      // Serial.println(inputMessage);
      preferences.putString("WIFIpass", inputMessage);  // Save entered WiFi password
      delay(250);
      preferences.end();
    } else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    request->send_P(200, "text/html", saved_html);
    delay(1000);
    ESP.restart();
  });

  server.on("/start", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/html", config_html);
  });

  server.on("/connecttest.txt", [](AsyncWebServerRequest* request) {
    request->redirect(captiveportalURL);
  });
  server.on("msftconnecttest.com", [](AsyncWebServerRequest* request) {
    request->redirect(captiveportalURL);
  });
  server.on("/fwlink", [](AsyncWebServerRequest* request) {
    request->redirect(captiveportalURL);
  });
  server.on("/wpad.dat", [](AsyncWebServerRequest* request) {
    request->send(404);
  });
  server.on("/generate_204", [](AsyncWebServerRequest* request) {
    request->redirect(captiveportalURL);
  });
  server.on("/redirect", [](AsyncWebServerRequest* request) {
    request->redirect(captiveportalURL);
  });
  server.on("/hotspot-detect.html", [](AsyncWebServerRequest* request) {
    request->redirect(captiveportalURL);
  });
  server.on("/canonical.html", [](AsyncWebServerRequest* request) {
    request->redirect(captiveportalURL);
  });
  server.on("/success.txt", [](AsyncWebServerRequest* request) {
    request->send(200);
  });
  server.on("/ncsi.txt", [](AsyncWebServerRequest* request) {
    request->redirect(captiveportalURL);
  });
  server.on("/chrome-variations/seed", [](AsyncWebServerRequest* request) {
    request->send(200);
  });
  server.on("/service/update2/json", [](AsyncWebServerRequest* request) {
    request->send(200);
  });
  server.on("/chat", [](AsyncWebServerRequest* request) {
    request->send(404);
  });
  server.on("/startpage", [](AsyncWebServerRequest* request) {
    request->redirect(captiveportalURL);
  });
  server.on("/favicon.ico", [](AsyncWebServerRequest* request) {
    request->send(404);
  });

  server.on("/", HTTP_ANY, [](AsyncWebServerRequest* request) {
    AsyncWebServerResponse* response = request->beginResponse(200, "text/html", index_html);
    response->addHeader("Cache-Control", "public,max-age=31536000");
    request->send(response);
    Serial.println("Served Basic HTML Page");
  });

  server.onNotFound([](AsyncWebServerRequest* request) {
    request->redirect(captiveportalURL);
    Serial.print("onnotfound ");
    Serial.print(request->host());
    Serial.print(" ");
    Serial.print(request->url());
    Serial.print(" sent redirect to " + captiveportalURL + "\n");
  });

  server.begin();
  Serial.println("Binary Matrix Clock Captive Portal web server started");
}


// ###########################################################################################################################################
// # Wifi setup and reconnect function that runs once at startup and during the loop function of the ESP:
// ###########################################################################################################################################
void WIFI_SETUP() {
  Serial.println(" ");
  esp_log_level_set("wifi", ESP_LOG_WARN);  // Disable WiFi debug warnings
  if (testTime == 0) {                      // If time text test mode is NOT used:
    String WIFIssid = preferences.getString("WIFIssid");
    bool WiFiConfigEmpty = false;
    if (WIFIssid == "") {
      // Serial.println("WIFIssid empty");
      WiFiConfigEmpty = true;
    } else {
      // Serial.print("WIFIssid = ");
      // Serial.println(WIFIssid);
    }
    String WIFIpass = preferences.getString("WIFIpass");
    if (WIFIpass == "") {
      // Serial.println("WIFIpass empty");
      WiFiConfigEmpty = true;
    } else {
      // Serial.print("WIFIpass = ");
      // Serial.println(WIFIpass);
    }
    if (WiFiConfigEmpty == true) {
      CaptivePotalSetup();
    } else {
      Serial.println("Try to connect to found WiFi configuration: ");
      WiFi.disconnect();
      int tryCount = 0;
      WiFi.mode(WIFI_STA);
      WiFi.begin((const char*)WIFIssid.c_str(), (const char*)WIFIpass.c_str());
      Serial.println("Connecting to WiFi " + String(WIFIssid));
      while (WiFi.status() != WL_CONNECTED) {
        tryCount = tryCount + 1;
        Serial.print("Connection try #: ");
        Serial.println(tryCount);
        if (tryCount == maxWiFiconnctiontries) {
          Serial.println("\n\nWIFI CONNECTION ERROR: If the connection still can not be established please check the WiFi settings or location of the device.\n\n");
          preferences.putString("WIFIssid", "");  // Reset entered WiFi ssid
          preferences.putString("WIFIpass", "");  // Reset entered WiFi password
          preferences.end();
          delay(250);
          Serial.println("WiFi settings deleted because in " + String(maxWiFiconnctiontries) + " tries the WiFi connection could not be established. Temporary Binary Matrix Clock access point will be started to reconfigure WiFi again.");
          ESP.restart();
        }
        delay(500);
      }
      Serial.println(" ");
      WiFIsetup = true;
      Serial.print("Successfully connected now to WiFi SSID: ");
      Serial.println(WiFi.SSID());
      Serial.println("IP: " + WiFi.localIP().toString());
      Serial.println("DNS: " + WiFi.dnsIP().toString());
      configNTPTime();      // NTP time setup
      setupWebInterface();  // Generate the configuration page
      updatenow = true;     // Update the display 1x after startup
      update_display();     // Update LED display
      setupOTAupate();      // ESP32 OTA update
      Serial.println("######################################################################");
      Serial.println("# Web interface online at: http://" + IpAddress2String(WiFi.localIP()));
      Serial.println("######################################################################");
      Serial.println("# Binary Matrix Clock startup finished...");
      Serial.println("######################################################################");
      Serial.println(" ");
    }
  }
}

// ###########################################################################################################################################
// # ESP32 OTA update:
// ###########################################################################################################################################
const char otaserverIndex[] PROGMEM = R"=====(
  <!DOCTYPE html><html><head><title>Binary Matrix Clock</title></head>
      <style>
      body {
      padding: 25px;
      font-size: 25px;
      background-color: black;
      color: white;
      }
      </style>
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <body>
    <form method='POST' action='/update' enctype='multipart/form-data'>
      <center><b><h1>Binary Matrix Clock software update</h1></b>
      <h2>Please select the in the Arduino IDE > "Sketch" ><br/>"Export Compiled Binary (Alt+Ctrl+S)"<br/>to generate the required .BIN file.<br/>
      Use the "Update" button 1x to start the update.<br/><br/>Binary Matrix Clock will restart automatically.</h2><br/>
      <input type='file' name='update'>       <input type='submit' value='Update'>
     </center></form></body>
  </html>
 )=====";


const char otaNOK[] PROGMEM = R"=====(
  <!DOCTYPE html><html><head><title>Binary Matrix Clock</title></head>
          <style>
      body {
      padding: 25px;
      font-size: 25px;
      background-color: black;
      color: white;
      }
      </style>
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
   <style>
    .button {
      display: inline-block;
      padding: 15px 25px;
      font-size: 24px;
      cursor: pointer;
      text-align: center;
      text-decoration: none;
      outline: none;
      color: #fff;
      background-color: #4CAF50;
      border: none;
      border-radius: 15px;
      box-shadow: 0 9px #999;
    }
    .button:hover {background-color: #3e8e41}
    .button:active {
      background-color: #3e8e41;
      box-shadow: 0 5px #666;
      transform: translateY(4px);
    }
    </style>
    <body>
      <center><b><h1>Binary Matrix Clock software update</h1></b>
      <h2>ERROR: Software update FAILED !!!<br/><br/>Binary Matrix Clock will restart automatically.</h2><br/>
      </center></body>
  </html>
 )=====";


const char otaOK[] PROGMEM = R"=====(
  <!DOCTYPE html><html><head><title>Binary Matrix Clock</title></head>
          <style>
      body {
      padding: 25px;
      font-size: 25px;
      background-color: black;
      color: white;
      }
      </style>
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
   <style>
    .button {
      display: inline-block;
      padding: 15px 25px;
      font-size: 24px;
      cursor: pointer;
      text-align: center;
      text-decoration: none;
      outline: none;
      color: #fff;
      background-color: #4CAF50;
      border: none;
      border-radius: 15px;
      box-shadow: 0 9px #999;
    }
    .button:hover {background-color: #3e8e41}
    .button:active {
      background-color: #3e8e41;
      box-shadow: 0 5px #666;
      transform: translateY(4px);
    }
    </style>
    <body>
      <center><b><h1>Binary Matrix Clock software update</h1></b>
      <h2>Software update done =)<br/><br/>Binary Matrix Clock will restart automatically.</h2><br/>
      </center></body>
  </html>
 )=====";


void setupOTAupate() {
  otaserver.on("/", HTTP_GET, []() {
    otaserver.sendHeader("Connection", "close");
    otaserver.send(200, "text/html", otaserverIndex);
  });

  otaserver.on(
    "/update", HTTP_POST, []() {
      otaserver.sendHeader("Connection", "close");
      if (Update.hasError()) {
        otaserver.send(200, "text/html", otaNOK);
      } else {
        otaserver.send(200, "text/html", otaOK);
      }
      delay(3000);
      ESP.restart();
    },
    []() {
      HTTPUpload& upload = otaserver.upload();
      if (upload.status == UPLOAD_FILE_START) {
        Serial.setDebugOutput(true);
        Serial.printf("Update: %s\n", upload.filename.c_str());
        if (!Update.begin()) {
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_WRITE) {
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) {
          Serial.printf("Update success: %u\nRebooting...\n", upload.totalSize);
        } else {
          Update.printError(Serial);
        }
        Serial.setDebugOutput(false);
      } else {
        Serial.printf("Update failed unexpectedly (likely broken connection): status=%d\n", upload.status);
      }
    });
  otaserver.begin();
}


// ###########################################################################################################################################
// # EOF - You have successfully reached the end of the code - well done ;-)
// ###########################################################################################################################################