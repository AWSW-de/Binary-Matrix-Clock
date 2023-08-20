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
// # Compatible with WordClock version: V1.0.0
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
// # Hardware settings:
// ###########################################################################################################################################
#define LEDPIN 32      // Arduino-Pin connected to the NeoPixels
#define NUMPIXELS 256  // How many NeoPixels are attached to the Arduino
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);


// ###########################################################################################################################################
// # LED defaults:
// ###########################################################################################################################################
int redVal_back_default = 0;      // Default background color RED
int greenVal_back_default = 0;    // Default background color GREEN
int blueVal_back_default = 0;     // Default background color BLUE
int redVal_time_default = 0;      // Default time color RED
int greenVal_time_default = 255;  // Default time color GREEN
int blueVal_time_default = 255;   // Default time color BLUE
int intensity_day_default = 22;   // LED intensity (0..255) in day mode   - Important note: Check power consumption and used power supply capabilities!
int intensity_night_default = 5;  // LED intensity (0..255) in day mode   - Important note: Check power consumption and used power supply capabilities!
int usenightmode_default = 1;     // Use the night mode to reduce LED intensity during set times
int day_time_start_default = 7;   // Define day mode start --> time before is then night mode if used
int day_time_stop_default = 22;   // Define day mode end --> time after is then night mode if used


// ###########################################################################################################################################
// # Various default settings:
// ###########################################################################################################################################
int maxWiFiconnctiontries = 90;  // Maximum connection tries to logon to the set WiFi. After the amount of tries is reached the WiFi settings will be deleted!


// ###########################################################################################################################################
// # Variables declaration:
// ###########################################################################################################################################
#define DEFAULT_AP_NAME "BinaryClock"  // WiFi access point name of the ESP32
const char* hostname = "BINARYCLOCK";  // Hostname to be set in your router


// ###########################################################################################################################################
// # NTP time server settings:
// ###########################################################################################################################################
const char* Timezone = "CET-1CEST,M3.5.0,M10.5.0/3";  // You can check a list of timezone string variables here:  https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
const char* NTPserver = "pool.ntp.org";               // Time server address. Choose the closest one to you here: https://gist.github.com/mutin-sa/eea1c396b1e610a2da1e5550d94b0453


// ###########################################################################################################################################
// # Test functions:
// ###########################################################################################################################################
int debugtexts = 1;        // Show more debug information in serial monitor
int testTime = 0;          // LED time text output test
int testDayNightmode = 0;  // Test the day/night mode settings
// Test a special time:
int testspecialtime = 0;  // Use test a special time function
int test_hour = 23;       // Test a special time: hour
int test_minute = 58;     // Test a special time: minute
int test_second = 59;     // Test a special time: second


// ###########################################################################################################################################
// # LED intensity setting:
// ###########################################################################################################################################
#define LEDintensityLIMIT 64  // off 255 max. !!! Do NOT change this if you don't know how much power drain this may cause to avoid damage !!! \
                              // Limit the intensity level to be able to select in the configuration to avoid to much power drain and to avoid hardware failures over time. \
                              // In case you want to use the device over longer times with a higher intensity or even higher, you will need to consider a much more \
                              // powerful power supply, a better cable to connect the device and you have to power the matrix directly to 5V of the power supply. \
                              // Otherwise the ESP32 can fail over time. Therefore this setting is limited and should not be raised. !!! You have been warned !!! \
                              // In case you really need a higher value, you need to make sure to power the matrix directly so that the power is not served over the ESP32 \
                              // to the matrix to avoid its damage over time. \
                              // Please think about if you really need such a high intensity value. The WordClocks used from me run at a maximum intensity of 33 which is \
                              // really bright in my eyes and all levels above 64 i could not see really an advantage anymore that the display is better able to view... \
                              // !!! Make sure to use propper components !!!


// ###########################################################################################################################################
// # EOF - You have successfully reached the end of the code - well done ;-)
// ###########################################################################################################################################