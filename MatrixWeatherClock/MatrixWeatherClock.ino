/*=============================================================================
 =======                            INCLUDES                            =======
 =============================================================================*/
#include <PxMatrix.h>
#include <Ticker.h>
#include <TimeLib.h>
#include <Timezone.h> 
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>          // Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>   // Local WebServer used to serve the configuration portal
#include <ESP8266HTTPClient.h>  // HTTPClient for web requests
#include <WiFiManager.h>        // WiFi Configuration Magic
#include <WiFiUdp.h>
#include <EEPROM.h>

/*=============================================================================
 =======               DEFINES & MACROS FOR GENERAL PURPOSE             =======
 =============================================================================*/
// #undef when PIR range problem is solved
#define DISPLAY_ON_PERMANENTLY

// clock appearance
#undef CLOCK_DISPLAY_SECONDS

// rounding float to integer
#define ROUND_INT(d) ((int) ((d) + ((d) > 0 ? 0.5 : -0.5)))

// access point
#define AP_NAME "matrix_weather_clock"
#define AP_TIMEOUT 300

// general purpose
#define DISPLAY_WIDTH          32
#define DISPLAY_HEIGHT         16
#define ICON_WIDTH              9
#define ICON_HEIGHT             9
#define FONT_WIDTH              3
#define FONT_HEIGHT             6
#define NBR_DIGITS             10
#define NBR_ICONS              61
#define UPDATE_PERIOD        6000
#define NTP_PACKET_SIZE        48
#define SEVENTYYEARS   2208988800UL
#define PIR_TIME_MAX         6000
#define NBR_MENUS               3
#define LINE0_END               9
#define WEBPAGE_BUFFER_SIZE  3500
#define WEEK_SELECTIONS         5
#define DOW_SELECTIONS          7
#define MONTH_SELECTIONS       12

// EEPROM
#define OWM_API_KEY_OFFSET         0
#define OWM_API_KEY_LEN           32
#define OWM_CITY_OFFSET           32
#define OWM_CITY_LEN              32
#define OWM_UNITS_OFFSET          64
#define OWM_UNITS_LEN              8
#define NTP_SERVER_OFFSET         72
#define NTP_SERVER_LEN            32     
#define DT_WEEK_OFFSET           104  
#define DT_WEEK_LEN                1
#define DT_DOW_OFFSET            105
#define DT_DOW_LEN                 1
#define DT_MONTH_OFFSET          106
#define DT_MONTH_LEN               1
#define DT_HOUR_OFFSET           107
#define DT_HOUR_LEN                1
#define DT_OFFSET_OFFSET         108
#define DT_OFFSET_LEN              4
#define ST_WEEK_OFFSET           112  
#define ST_WEEK_LEN                1
#define ST_DOW_OFFSET            113
#define ST_DOW_LEN                 1
#define ST_MONTH_OFFSET          114
#define ST_MONTH_LEN               1
#define ST_HOUR_OFFSET           115
#define ST_HOUR_LEN                1
#define ST_OFFSET_OFFSET         116
#define ST_OFFSET_LEN              4
#define PIR_TIME_OFFSET          120
#define PIR_TIME_LEN               2

// used colors
#define RGB(r, g, b) ((((uint16_t)r & 0x00F8) << 8) | (((uint16_t)g & 0x00FC) << 3) | ((uint16_t)b >> 3))
#define BLK RGB(0, 0, 0)        // Black
#define CYN RGB(127, 255, 255)  // Light Cyan
#define GRY RGB(24, 24, 24)     // Dark Grey
#define YEL RGB(255, 255, 0)    // Yellow
#define BLU RGB(0, 0, 255)      // Blue
#define RED RGB(255, 0, 0)      // Red
#define GRN RGB(0, 255, 0)      // Green
#define ORG RGB(255, 127, 0)    // Orange
#define WHT RGB(255, 255, 255)  // White

// Port Pins for LED Matrix @ ESP8266
#define P_LAT 16
#define P_A    5
#define P_B    4
#define P_C   15
//#define P_D   12
//#define P_E    0
#define P_OE   2

/*=============================================================================
 =======                       CONSTANTS  &  TYPES                      =======
 =============================================================================*/
typedef struct
{
  uint8_t digit_old;
  uint8_t digit_new;
  uint8_t digit_show_old;
  uint8_t digit_show_new;
  uint8_t digit_anim_step;
  uint8_t digit_xoffset;
} digit_anim_t;

typedef struct
{
  int             icon_number;
  const uint16_t *icon_day;
  const uint16_t *icon_night;  
}icon_list_t;

const uint16_t icon1d[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, CYN, CYN, BLK, BLK, YEL, YEL, BLK,
  BLK, CYN, CYN, CYN, CYN, YEL, YEL, YEL, YEL,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, YEL, YEL,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN,
  BLK, CYN, YEL, YEL, CYN, BLU, CYN, CYN, BLU,
  BLK, BLK, YEL, CYN, CYN, BLU, CYN, BLU, BLK,
  BLK, YEL, YEL, BLK, BLU, BLK, BLK, BLU, BLK,
  BLK, YEL, BLK, BLK, BLU, BLK, BLU, BLK, BLK,
  YEL, BLK, BLK, BLU, BLK, BLK, BLK, BLK, BLK
};

const uint16_t icon1n[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, CYN, CYN, BLK, BLK, GRY, GRY, BLK,
  BLK, CYN, CYN, CYN, CYN, GRY, GRY, GRY, GRY,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, GRY, GRY,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN,
  BLK, CYN, YEL, YEL, CYN, BLU, CYN, CYN, BLU,
  BLK, BLK, YEL, CYN, CYN, BLU, CYN, BLU, BLK,
  BLK, YEL, YEL, BLK, BLU, BLK, BLK, BLU, BLK,
  BLK, YEL, BLK, BLK, BLU, BLK, BLU, BLK, BLK,
  YEL, BLK, BLK, BLU, BLK, BLK, BLK, BLK, BLK
};

const uint16_t icon2[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, YEL,
  BLK, BLK, BLK, YEL, YEL, BLK, BLK, YEL, BLK,
  BLK, BLK, YEL, YEL, BLK, BLK, YEL, BLK, BLK,
  BLK, YEL, YEL, BLK, BLK, YEL, YEL, YEL, YEL,
  YEL, YEL, YEL, YEL, BLK, BLK, BLK, YEL, BLK,
  BLK, BLK, YEL, BLK, BLK, BLK, YEL, BLK, BLK,
  BLK, YEL, BLK, BLK, BLK, BLK, BLK, BLK, BLK,
  YEL, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK,
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK
};

const uint16_t icon3d[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, CYN, CYN, BLK, BLK, YEL, YEL, BLK,
  BLK, CYN, CYN, CYN, CYN, YEL, YEL, YEL, YEL,
  CYN, BLU, BLU, CYN, BLU, BLU, CYN, YEL, YEL,
  CYN, BLU, BLU, CYN, BLU, BLU, CYN, CYN, CYN,
  BLK, CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN,
  BLK, BLK, CYN, CYN, CYN, BLU, BLU, BLK, BLK,
  BLK, BLK, BLK, BLK, BLK, BLU, BLU, BLK, BLK,
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK,
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK
};

const uint16_t icon3n[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, CYN, CYN, BLK, BLK, GRY, GRY, BLK,
  BLK, CYN, CYN, CYN, CYN, GRY, GRY, GRY, GRY,
  CYN, BLU, BLU, CYN, BLU, BLU, CYN, GRY, GRY,
  CYN, BLU, BLU, CYN, BLU, BLU, CYN, CYN, CYN,
  BLK, CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN,
  BLK, BLK, CYN, CYN, CYN, BLU, BLU, BLK, BLK,
  BLK, BLK, BLK, BLK, BLK, BLU, BLU, BLK, BLK,
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK,
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK
};

const uint16_t icon4d[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, CYN, CYN, BLK, BLK, YEL, YEL, BLK,
  BLK, CYN, CYN, CYN, CYN, YEL, YEL, YEL, YEL,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, YEL, YEL,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN,
  BLK, CYN, CYN, BLU, CYN, BLU, CYN, CYN, BLU,
  BLK, BLK, BLU, CYN, CYN, BLU, CYN, BLU, BLK,
  BLK, BLK, BLU, BLK, BLU, BLK, BLK, BLU, BLK,
  BLK, BLU, BLK, BLK, BLU, BLK, BLU, BLK, BLK,
  BLK, BLK, BLK, BLU, BLK, BLK, BLK, BLK, BLK
};

const uint16_t icon4n[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, CYN, CYN, BLK, BLK, GRY, GRY, BLK,
  BLK, CYN, CYN, CYN, CYN, GRY, GRY, GRY, GRY,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, GRY, GRY,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN,
  BLK, CYN, CYN, BLU, CYN, BLU, CYN, CYN, BLU,
  BLK, BLK, BLU, CYN, CYN, BLU, CYN, BLU, BLK,
  BLK, BLK, BLU, BLK, BLU, BLK, BLK, BLU, BLK,
  BLK, BLU, BLK, BLK, BLU, BLK, BLU, BLK, BLK,
  BLK, BLK, BLK, BLU, BLK, BLK, BLK, BLK, BLK
};

const uint16_t icon5d[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, CYN, CYN, BLK, BLK, YEL, YEL, BLK,
  BLK, CYN, CYN, CYN, CYN, YEL, YEL, YEL, YEL,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, YEL, YEL,
  CYN, CYN, CYN, BLU, CYN, CYN, CYN, CYN, BLU,
  BLK, CYN, CYN, CYN, CYN, BLU, CYN, CYN, CYN,
  BLK, BLK, BLU, CYN, CYN, CYN, CYN, BLU, BLK,
  BLK, BLK, BLK, BLK, BLU, BLK, BLK, BLK, BLK,
  BLK, BLU, BLK, BLK, BLK, BLK, BLU, BLK, BLK,
  BLK, BLK, BLK, BLU, BLK, BLK, BLK, BLK, BLK
};

const uint16_t icon5n[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, CYN, CYN, BLK, BLK, GRY, GRY, BLK,
  BLK, CYN, CYN, CYN, CYN, GRY, GRY, GRY, GRY,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, GRY, GRY,
  CYN, CYN, CYN, BLU, CYN, CYN, CYN, CYN, BLU,
  BLK, CYN, CYN, CYN, CYN, BLU, CYN, CYN, CYN,
  BLK, BLK, BLU, CYN, CYN, CYN, CYN, BLU, BLK,
  BLK, BLK, BLK, BLK, BLU, BLK, BLK, BLK, BLK,
  BLK, BLU, BLK, BLK, BLK, BLK, BLU, BLK, BLK,
  BLK, BLK, BLK, BLU, BLK, BLK, BLK, BLK, BLK
};

const uint16_t icon6d[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, CYN, CYN, BLK, BLK, YEL, YEL, BLK,
  BLK, CYN, CYN, CYN, CYN, YEL, YEL, YEL, YEL,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, YEL, YEL,
  CYN, CYN, CYN, BLU, CYN, CYN, CYN, CYN, BLU,
  BLK, CYN, CYN, BLU, CYN, BLU, CYN, CYN, BLU,
  BLK, BLK, CYN, CYN, CYN, BLU, CYN, BLK, BLK,
  BLK, BLK, BLU, BLK, BLK, BLK, BLK, BLU, BLK,
  BLK, BLU, BLK, BLK, BLU, BLK, BLU, BLK, BLK,
  BLK, BLK, BLK, BLU, BLK, BLK, BLK, BLK, BLK
};

const uint16_t icon6n[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, CYN, CYN, BLK, BLK, GRY, GRY, BLK,
  BLK, CYN, CYN, CYN, CYN, GRY, GRY, GRY, GRY,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, GRY, GRY,
  CYN, CYN, CYN, BLU, CYN, CYN, CYN, CYN, BLU,
  BLK, CYN, CYN, BLU, CYN, BLU, CYN, CYN, BLU,
  BLK, BLK, CYN, CYN, CYN, BLU, CYN, BLK, BLK,
  BLK, BLK, BLU, BLK, BLK, BLK, BLK, BLU, BLK,
  BLK, BLU, BLK, BLK, BLU, BLK, BLU, BLK, BLK,
  BLK, BLK, BLK, BLU, BLK, BLK, BLK, BLK, BLK
};

const uint16_t icon7d[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, CYN, CYN, BLK, BLK, YEL, YEL, BLK,
  BLK, CYN, CYN, CYN, CYN, YEL, YEL, YEL, YEL,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, YEL, YEL,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN, BLU,
  BLK, CYN, YEL, YEL, CYN, BLU, CYN, CYN, BLU,
  BLK, BLK, YEL, CYN, CYN, BLU, CYN, BLK, BLK,
  BLK, YEL, YEL, BLK, BLK, BLK, BLK, BLU, BLK,
  BLK, YEL, BLK, BLK, BLU, BLK, BLU, BLK, BLK,
  YEL, BLK, BLK, BLU, BLK, BLK, BLK, BLK, BLK
};

const uint16_t icon7n[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, CYN, CYN, BLK, BLK, GRY, GRY, BLK,
  BLK, CYN, CYN, CYN, CYN, GRY, GRY, GRY, GRY,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, GRY, GRY,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN, BLU,
  BLK, CYN, YEL, YEL, CYN, BLU, CYN, CYN, BLU,
  BLK, BLK, YEL, CYN, CYN, BLU, CYN, BLK, BLK,
  BLK, YEL, YEL, BLK, BLK, BLK, BLK, BLU, BLK,
  BLK, YEL, BLK, BLK, BLU, BLK, BLU, BLK, BLK,
  YEL, BLK, BLK, BLU, BLK, BLK, BLK, BLK, BLK
};

const uint16_t icon8d[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, CYN, CYN, BLK, BLK, YEL, YEL, BLK,
  BLK, CYN, CYN, CYN, CYN, YEL, YEL, YEL, YEL,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, YEL, YEL,
  CYN, CYN, CYN, CYN, BLU, CYN, CYN, CYN, CYN,
  BLK, CYN, BLU, CYN, CYN, CYN, BLU, CYN, CYN,
  BLK, BLK, CYN, CYN, BLU, CYN, CYN, BLK, BLK,
  BLK, BLK, BLU, BLK, BLK, BLK, BLU, BLK, BLK,
  BLK, BLK, BLK, BLK, BLU, BLK, BLK, BLK, BLK,
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK
};

const uint16_t icon8n[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, CYN, CYN, BLK, BLK, GRY, GRY, BLK,
  BLK, CYN, CYN, CYN, CYN, GRY, GRY, GRY, GRY,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, GRY, GRY,
  CYN, CYN, CYN, CYN, BLU, CYN, CYN, CYN, CYN,
  BLK, CYN, BLU, CYN, CYN, CYN, BLU, CYN, CYN,
  BLK, BLK, CYN, CYN, BLU, CYN, CYN, BLK, BLK,
  BLK, BLK, BLU, BLK, BLK, BLK, BLU, BLK, BLK,
  BLK, BLK, BLK, BLK, BLU, BLK, BLK, BLK, BLK,
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK
};

const uint16_t icon9d[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, CYN, CYN, BLK, BLK, YEL, YEL, BLK,
  BLK, CYN, CYN, CYN, CYN, YEL, YEL, YEL, YEL,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, YEL, YEL,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN,
  BLK, CYN, CYN, CYN, CYN, BLU, CYN, CYN, CYN,
  BLK, BLK, BLU, CYN, CYN, CYN, CYN, BLU, BLK,
  BLK, BLK, BLK, BLK, BLU, BLK, BLK, BLK, BLK,
  BLK, BLU, BLK, BLK, BLK, BLK, BLU, BLK, BLK,
  BLK, BLK, BLK, BLU, BLK, BLK, BLK, BLK, BLK
};

const uint16_t icon9n[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, CYN, CYN, BLK, BLK, GRY, GRY, BLK,
  BLK, CYN, CYN, CYN, CYN, GRY, GRY, GRY, GRY,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, GRY, GRY,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN,
  BLK, CYN, CYN, CYN, CYN, BLU, CYN, CYN, CYN,
  BLK, BLK, BLU, CYN, CYN, CYN, CYN, BLU, BLK,
  BLK, BLK, BLK, BLK, BLU, BLK, BLK, BLK, BLK,
  BLK, BLU, BLK, BLK, BLK, BLK, BLU, BLK, BLK,
  BLK, BLK, BLK, BLU, BLK, BLK, BLK, BLK, BLK
};

const uint16_t icon10[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, CYN, CYN, CYN, CYN, BLK, BLK, BLK,
  BLK, CYN, CYN, CYN, CYN, CYN, CYN, CYN, BLK,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN,
  BLK, CYN, CYN, CYN, CYN, CYN, CYN, CYN, BLK,
  BLK, BLK, CYN, CYN, GRY, GRY, GRY, BLK, BLK,
  BLK, BLK, BLK, GRY, GRY, GRY, BLK, BLK, BLK,
  BLK, BLK, GRY, GRY, GRY, GRY, BLK, BLK, BLK,
  BLK, BLK, BLK, GRY, GRY, BLK, BLK, BLK, BLK
};

const uint16_t icon11d[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, BLK, BLK, YEL, BLK, BLK, BLK, BLK,
  BLK, YEL, BLK, BLK, BLK, BLK, BLK, YEL, BLK,
  BLK, BLK, BLK, YEL, YEL, YEL, BLK, BLK, BLK,
  BLK, BLK, YEL, YEL, YEL, YEL, YEL, BLK, BLK,
  YEL, BLK, YEL, YEL, YEL, YEL, YEL, BLK, YEL,
  GRY, GRY, GRY, GRY, BLK, GRY, BLK, GRY, GRY,
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK,
  BLK, GRY, BLK, GRY, GRY, BLK, GRY, GRY, BLK,
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK
};

const uint16_t icon11n[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK,
  BLK, BLK, BLK, GRY, GRY, GRY, BLK, BLK, BLK,
  BLK, BLK, GRY, GRY, GRY, GRY, GRY, BLK, BLK,
  BLK, GRY, GRY, GRY, GRY, GRY, GRY, GRY, BLK,
  BLK, GRY, GRY, GRY, GRY, GRY, GRY, GRY, BLK,
  GRY, GRY, GRY, GRY, BLK, GRY, BLK, GRY, GRY,
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK,
  BLK, GRY, BLK, GRY, GRY, BLK, GRY, GRY, BLK,
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK
};

const uint16_t icon12[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK,
  GRY, GRY, BLK, GRY, GRY, GRY, GRY, BLK, GRY,
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK,
  BLK, GRY, BLK, GRY, GRY, BLK, GRY, GRY, BLK,
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK,
  GRY, GRY, GRY, GRY, BLK, GRY, BLK, GRY, GRY,
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK,
  BLK, GRY, BLK, GRY, GRY, BLK, GRY, GRY, BLK,
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK
};

const uint16_t icon13d[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, CYN, CYN, BLK, BLK, YEL, YEL, BLK,
  BLK, CYN, CYN, CYN, CYN, YEL, YEL, YEL, YEL,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, YEL, YEL,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN,
  BLK, BLU, BLU, BLU, BLU, BLU, BLU, BLU, CYN,
  BLK, BLK, CYN, CYN, CYN, CYN, CYN, BLK, BLK,
  BLU, BLU, BLU, BLU, BLU, BLU, BLU, BLK, BLK,
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK,
  BLK, BLK, BLU, BLU, BLU, BLU, BLU, BLU, BLU
};

const uint16_t icon13n[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, CYN, CYN, BLK, BLK, GRY, GRY, BLK,
  BLK, CYN, CYN, CYN, CYN, GRY, GRY, GRY, GRY,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, GRY, GRY,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN,
  BLK, BLU, BLU, BLU, BLU, BLU, BLU, BLU, CYN,
  BLK, BLK, CYN, CYN, CYN, CYN, CYN, BLK, BLK,
  BLU, BLU, BLU, BLU, BLU, BLU, BLU, BLK, BLK,
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK,
  BLK, BLK, BLU, BLU, BLU, BLU, BLU, BLU, BLU
};

const uint16_t icon14d[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, CYN, CYN, BLK, BLK, YEL, YEL, BLK,
  BLK, CYN, CYN, CYN, CYN, YEL, YEL, YEL, YEL,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, YEL, YEL,
  CYN, CYN, CYN, CYN, BLU, BLU, CYN, CYN, CYN,
  BLU, BLU, BLU, BLU, BLU, BLU, CYN, CYN, CYN,
  BLK, BLK, CYN, CYN, CYN, CYN, CYN, BLK, BLK,
  BLU, BLU, BLU, BLU, BLK, BLK, BLK, BLK, BLK,
  BLK, BLK, BLU, BLU, BLK, BLK, BLK, BLK, BLK,
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK
};

const uint16_t icon14n[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, CYN, CYN, BLK, BLK, GRY, GRY, BLK,
  BLK, CYN, CYN, CYN, CYN, GRY, GRY, GRY, GRY,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, GRY, GRY,
  CYN, CYN, CYN, CYN, BLU, BLU, CYN, CYN, CYN,
  BLU, BLU, BLU, BLU, BLU, BLU, CYN, CYN, CYN,
  BLK, BLK, CYN, CYN, CYN, CYN, CYN, BLK, BLK,
  BLU, BLU, BLU, BLU, BLK, BLK, BLK, BLK, BLK,
  BLK, BLK, BLU, BLU, BLK, BLK, BLK, BLK, BLK,
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK
};

const uint16_t icon15[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, BLK, GRY, GRY, GRY, GRY, GRY, GRY,
  BLK, BLK, GRY, BLK, BLK, BLK, BLK, BLK, GRY,
  BLK, BLK, GRY, GRY, GRY, GRY, GRY, GRY, BLK,
  BLK, GRY, BLK, BLK, BLK, BLK, BLK, GRY, BLK,
  BLK, GRY, GRY, GRY, GRY, GRY, GRY, BLK, BLK,
  GRY, BLK, BLK, BLK, BLK, GRY, BLK, BLK, BLK,
  BLK, GRY, GRY, GRY, GRY, BLK, BLK, BLK, BLK,
  BLK, GRY, BLK, BLK, GRY, BLK, BLK, BLK, BLK,
  BLK, BLK, GRY, GRY, GRY, BLK, BLK, BLK, BLK
};

const uint16_t icon16d[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, BLK, BLK, YEL, BLK, BLK, BLK, BLK,
  BLK, YEL, BLK, BLK, BLK, BLK, BLK, YEL, BLK,
  BLK, BLK, BLK, YEL, YEL, YEL, BLK, BLK, BLK,
  BLK, BLK, YEL, YEL, YEL, YEL, YEL, BLK, BLK,
  YEL, BLK, YEL, YEL, YEL, YEL, YEL, BLK, YEL,
  BLK, BLK, YEL, YEL, YEL, YEL, YEL, BLK, BLK,
  BLK, BLK, BLK, YEL, YEL, YEL, BLK, BLK, BLK,
  BLK, YEL, BLK, BLK, BLK, BLK, BLK, YEL, BLK,
  BLK, BLK, BLK, BLK, YEL, BLK, BLK, BLK, BLK
};

const uint16_t icon16n[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK,
  BLK, BLK, BLK, GRY, GRY, GRY, BLK, BLK, BLK,
  BLK, BLK, GRY, GRY, GRY, GRY, GRY, BLK, BLK,
  BLK, GRY, GRY, GRY, GRY, GRY, GRY, GRY, BLK,
  BLK, GRY, GRY, GRY, GRY, GRY, GRY, GRY, BLK,
  BLK, GRY, GRY, GRY, GRY, GRY, GRY, GRY, BLK,
  BLK, BLK, GRY, GRY, GRY, GRY, GRY, BLK, BLK,
  BLK, BLK, BLK, GRY, GRY, GRY, BLK, BLK, BLK,
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK
};

const uint16_t icon17d[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, CYN, CYN, BLK, BLK, YEL, YEL, BLK,
  BLK, CYN, CYN, CYN, CYN, YEL, YEL, YEL, YEL,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, YEL, YEL,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN,
  BLK, BLU, BLU, BLU, BLU, CYN, CYN, CYN, CYN,
  BLK, BLK, CYN, CYN, CYN, CYN, CYN, BLK, BLK,
  BLU, BLU, BLU, BLU, BLK, BLK, BLK, BLK, BLK,
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK,
  BLK, BLK, BLU, BLU, BLU, BLU, BLK, BLK, BLK
};

const uint16_t icon17n[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, CYN, CYN, BLK, BLK, GRY, GRY, BLK,
  BLK, CYN, CYN, CYN, CYN, GRY, GRY, GRY, GRY,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, GRY, GRY,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN,
  BLK, BLU, BLU, BLU, BLU, CYN, CYN, CYN, CYN,
  BLK, BLK, CYN, CYN, CYN, CYN, CYN, BLK, BLK,
  BLU, BLU, BLU, BLU, BLK, BLK, BLK, BLK, BLK,
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK,
  BLK, BLK, BLU, BLU, BLU, BLU, BLK, BLK, BLK
};

const uint16_t icon18[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, CYN, CYN, BLK, BLU, BLU, BLU, BLK,
  BLK, CYN, CYN, CYN, CYN, BLU, BLU, BLU, BLU,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, BLU, BLK,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN,
  BLK, CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN,
  BLK, BLK, CYN, CYN, CYN, CYN, CYN, BLK, BLK,
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK,
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK,
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK
};

const uint16_t icon19[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, BLK, BLK, BLK, BLU, BLK, BLK, BLK,
  BLK, BLK, BLK, BLK, BLU, BLK, BLK, BLK, BLK,
  BLK, BLK, BLK, BLU, BLK, BLK, BLK, BLK, BLK,
  BLK, BLK, BLK, BLU, BLU, BLK, BLK, BLK, BLK,
  BLK, BLK, BLK, BLU, BLK, BLU, BLK, BLK, BLK,
  BLK, BLK, BLK, BLK, BLU, BLU, BLK, BLK, BLK,
  BLK, BLK, BLK, BLK, BLK, BLU, BLK, BLK, BLK,
  BLK, BLK, BLK, BLK, BLU, BLK, BLK, BLK, BLK,
  BLK, BLK, BLK, BLU, BLK, BLK, BLK, BLK, BLK
};

const uint16_t icon20[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, BLK, BLK, BLU, BLK, BLK, BLK, BLK,
  BLK, BLU, BLK, BLK, BLU, BLK, BLK, BLU, BLK,
  BLK, BLK, BLU, BLK, BLK, BLK, BLU, BLK, BLK,
  BLK, BLK, BLK, BLK, BLU, BLK, BLK, BLK, BLK,
  BLU, BLU, BLK, BLU, BLU, BLU, BLK, BLU, BLU,
  BLK, BLK, BLK, BLK, BLU, BLK, BLK, BLK, BLK,
  BLK, BLK, BLU, BLK, BLK, BLK, BLU, BLK, BLK,
  BLK, BLU, BLK, BLK, BLU, BLK, BLK, BLU, BLK,
  BLK, BLK, BLK, BLK, BLU, BLK, BLK, BLK, BLK
};

const uint16_t icon21d[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, BLK, BLK, YEL, BLK, BLK, BLK, BLK,
  BLK, YEL, BLK, BLK, BLK, BLK, BLK, YEL, BLK,
  BLK, BLK, BLK, YEL, YEL, YEL, BLK, BLK, BLK,
  BLK, BLK, YEL, YEL, YEL, YEL, YEL, BLK, BLK,
  YEL, BLK, YEL, YEL, YEL, YEL, YEL, BLK, YEL,
  BLK, RED, BLK, BLK, RED, BLK, BLK, RED, BLK,
  RED, BLK, BLK, RED, BLK, BLK, RED, BLK, BLK,
  BLK, RED, BLK, BLK, RED, BLK, BLK, RED, BLK,
  RED, BLK, BLK, RED, BLK, BLK, RED, BLK, BLK
};

const uint16_t icon21n[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK,
  BLK, BLK, BLK, GRY, GRY, GRY, BLK, BLK, BLK,
  BLK, BLK, GRY, GRY, GRY, GRY, GRY, BLK, BLK,
  BLK, GRY, GRY, GRY, GRY, GRY, GRY, GRY, BLK,
  BLK, GRY, GRY, GRY, GRY, GRY, GRY, GRY, BLK,
  BLK, RED, BLK, BLK, RED, BLK, BLK, RED, BLK,
  RED, BLK, BLK, RED, BLK, BLK, RED, BLK, BLK,
  BLK, RED, BLK, BLK, RED, BLK, BLK, RED, BLK,
  RED, BLK, BLK, RED, BLK, BLK, RED, BLK, BLK
};

const uint16_t icon22[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK,
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK,
  BLK, CYN, CYN, CYN, CYN, CYN, CYN, BLK, BLK,
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK,
  CYN, CYN, CYN, CYN, CYN, CYN, BLK, CYN, CYN,
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK,
  BLK, BLK, CYN, CYN, CYN, CYN, CYN, CYN, BLK,
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK,
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK
};

const uint16_t icon23d[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, CYN, CYN, BLK, BLK, YEL, YEL, BLK,
  BLK, CYN, CYN, CYN, CYN, YEL, YEL, YEL, YEL,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, YEL, YEL,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN,
  BLK, CYN, CYN, BLU, CYN, BLU, CYN, CYN, BLU,
  BLK, BLK, BLU, CYN, CYN, BLU, CYN, BLU, BLK,
  BLK, BLK, BLK, BLK, BLU, BLK, BLK, BLK, BLK,
  BLK, BLU, BLK, BLK, BLK, BLK, BLU, BLK, BLK,
  BLK, BLK, BLK, BLU, BLK, BLK, BLK, BLK, BLK
};

const uint16_t icon23n[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, CYN, CYN, BLK, BLK, GRY, GRY, BLK,
  BLK, CYN, CYN, CYN, CYN, GRY, GRY, GRY, GRY,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, GRY, GRY,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN,
  BLK, CYN, CYN, BLU, CYN, BLU, CYN, CYN, BLU,
  BLK, BLK, BLU, CYN, CYN, BLU, CYN, BLU, BLK,
  BLK, BLK, BLK, BLK, BLU, BLK, BLK, BLK, BLK,
  BLK, BLU, BLK, BLK, BLK, BLK, BLU, BLK, BLK,
  BLK, BLK, BLK, BLU, BLK, BLK, BLK, BLK, BLK
};

const uint16_t icon24[ICON_WIDTH * ICON_HEIGHT] = {
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK,
  BLK, BLK, BLK, BLK, BLK, BLK, CYN, CYN, BLK,
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, CYN,
  CYN, CYN, CYN, CYN, CYN, CYN, CYN, CYN, BLK,
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK,
  CYN, CYN, CYN, CYN, CYN, CYN, BLK, BLK, BLK,
  BLK, BLK, BLK, BLK, BLK, BLK, CYN, BLK, BLK,
  BLK, BLK, BLK, BLK, CYN, CYN, BLK, BLK, BLK,
  BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK
};

const icon_list_t icon_list[NBR_ICONS] = {
  // Gewitter
  {200, icon1d , icon1n }, // Gewitter mit leichtem Regen
  {201, icon1d , icon1n }, // Gewitter mit Regen
  {202, icon1d , icon1n }, // Gewitter mit starkem Regen
  {210, icon2  , icon2  }, // leichtes Gewitter
  {211, icon2  , icon2  }, // Gewitter
  {212, icon2  , icon2  }, // starkes Gewitter
  {221, icon2  , icon2  }, // phasenweises Gewitter
  {230, icon1d , icon1n }, // Gewitter mit leichtem Nieselregen
  {231, icon1d , icon1n }, // Gewitter mit Regen
  {232, icon1d , icon1n }, // Gewitter mit starkem Regen
  // Nieselregen
  {300, icon3d , icon3n }, // leichter Nieselregen
  {301, icon3d , icon3n }, // Nieselregen
  {302, icon4d , icon4n }, // starker Nieselregen
  {310, icon5d , icon5n }, // leichter Nieselregen/Regen
  {311, icon4d , icon4n }, // Nieselregen/Regen
  {312, icon4d , icon4n }, // starker Nieselregen/Regen 
  {313, icon6d , icon6n }, // Regenschauer und Nebel
  {314, icon4d , icon4n }, // starker Regenschauer und Nebel 
  {321, icon3d , icon3n }, // Regenschauer und Nieselregen
  // Regen
  {500, icon3d , icon3n }, // leichter Regen
  {501, icon4d , icon4n }, // mäßiger Regen
  {502, icon4d , icon4n }, // starker Regen
  {503, icon4d , icon4n }, // sehr starker Regen
  {504, icon4d , icon4n }, // extremer Regen
  {511, icon5d , icon5n }, // gefrierender Regen
  {520, icon6d , icon6n }, // leichter Regenschauer/Regen
  {521, icon6d , icon6n }, // Regenschauer
  {522, icon6d , icon6n }, // starker Regenschauer/Regen
  {531, icon7d , icon7n }, // phasenweise Regenschauer/Regen
  // Schnee
  {600, icon8d , icon8n }, // leichter Schneefall
  {601, icon8d , icon8n }, // Schneefall 
  {602, icon9d , icon9n }, // starker Schneefall
  {611, icon5d , icon5n }, // Schneeregen
  {612, icon5d , icon5n }, // Graupelschauer
  {615, icon5d , icon5n }, // leichter Regen und Schnee
  {616, icon5d , icon5n }, // Regen und Schnee
  {620, icon5d , icon5n }, // leichter Schneeregen
  {621, icon8d , icon8n }, // Schneeregen
  {622, icon8d , icon8n }, // starker Schneeregen
  // Atmosphäre
  {701, icon6d , icon6n }, // feiner Nebel/Dunst
  {711, icon10 , icon10 }, // Rauch
  {721 ,icon11d, icon11n}, // Dunst
  {731 ,icon12 , icon12 }, // Sand/Staubwirbel
  {741, icon13d, icon13n}, // Nebel
  {761 ,icon12 , icon12 }, // Staub
  {762 ,icon12 , icon12 }, // Vulkanasche
  {771, icon14d, icon14n}, // Wind-/Sturmböen
  {781, icon15,  icon15 }, // Tornado
  // Wolken
  {800, icon16d, icon16n}, // klarer Himmel
  {801, icon14d, icon14n}, // wenige Schleierwolken
  {802, icon14d, icon14n}, // aufgelockerte Bewölkung
  {803, icon17d, icon17n}, // bewölkt mit Aufhellungen
  {804, icon18 , icon18 }, // bewölkt
  // Extrem
  {900, icon15 , icon15 }, // Tornado
  {901, icon7d , icon7n }, // Tropensturm
  {902, icon19 , icon19 }, // Hurrikan
  {903, icon20 , icon20 }, // kalt
  {904, icon21d, icon21n}, // heiß
  {905, icon22 , icon22 }, // windig
  {906, icon23d, icon23n}, // Hagel
  // Ergänzende
  {957, icon24 , icon24 }  // starker Wind, kurz vor Sturm
};

const uint16_t digit[NBR_DIGITS][FONT_HEIGHT][FONT_WIDTH] = {
  {{BLK, BLK, BLK}, // 0
   {WHT, WHT, WHT},
   {WHT, BLK, WHT},
   {WHT, BLK, WHT},
   {WHT, BLK, WHT},
   {WHT, WHT, WHT}},
  {{BLK, BLK, BLK}, // 1
   {BLK, WHT, BLK},
   {WHT, WHT, BLK},
   {BLK, WHT, BLK},
   {BLK, WHT, BLK},
   {WHT, WHT, WHT}},
  {{BLK, BLK, BLK}, // 2
   {WHT, WHT, WHT},
   {BLK, BLK, WHT},
   {WHT, WHT, WHT},
   {WHT, BLK, BLK},
   {WHT, WHT, WHT}},
  {{BLK, BLK, BLK}, // 3
   {WHT, WHT, WHT},
   {BLK, BLK, WHT},
   {WHT, WHT, WHT},
   {BLK, BLK, WHT},
   {WHT, WHT, WHT}},
  {{BLK, BLK, BLK}, // 4
   {WHT, BLK, WHT},
   {WHT, BLK, WHT},
   {WHT, WHT, WHT},
   {BLK, BLK, WHT},
   {BLK, BLK, WHT}},
  {{BLK, BLK, BLK},  // 5
   {WHT, WHT, WHT},
   {WHT, BLK, BLK},
   {WHT, WHT, WHT},
   {BLK, BLK, WHT},
   {WHT, WHT, WHT}},
  {{BLK, BLK, BLK},  // 6
   {WHT, WHT, WHT},
   {WHT, BLK, BLK},
   {WHT, WHT, WHT},
   {WHT, BLK, WHT},
   {WHT, WHT, WHT}},
  {{BLK, BLK, BLK},  // 7
   {WHT, WHT, WHT},
   {BLK, BLK, WHT},
   {BLK, BLK, WHT},
   {BLK, BLK, WHT},
   {BLK, BLK, WHT}},
  {{BLK, BLK, BLK},  // 8
   {WHT, WHT, WHT},
   {WHT, BLK, WHT},
   {WHT, WHT, WHT},
   {WHT, BLK, WHT},
   {WHT, WHT, WHT}},
  {{BLK, BLK, BLK},  // 9
   {WHT, WHT, WHT},
   {WHT, BLK, WHT},
   {WHT, WHT, WHT},
   {BLK, BLK, WHT},
   {WHT, WHT, WHT}}
};

const uint16_t char_a[FONT_HEIGHT][FONT_WIDTH] = {
   {BLK, BLK, BLK},  // a
   {BLK, BLK, BLK},
   {WHT, WHT, BLK},
   {BLK, WHT, WHT},
   {WHT, BLK, WHT},
   {WHT, WHT, WHT} 
};

const uint16_t char_e[FONT_HEIGHT][FONT_WIDTH] = {
   {BLK, BLK, BLK},  // e
   {BLK, BLK, BLK},
   {BLK, WHT, WHT},
   {WHT, BLK, WHT},
   {WHT, WHT, BLK},
   {BLK, WHT, WHT} 
};

const uint16_t char_h[FONT_HEIGHT][FONT_WIDTH] = {
   {BLK, BLK, BLK},  // h
   {WHT, BLK, BLK},
   {WHT, WHT, BLK},
   {WHT, BLK, WHT},
   {WHT, BLK, WHT},
   {WHT, BLK, WHT} 
};

const uint16_t char_l[FONT_HEIGHT][FONT_WIDTH] = {
   {BLK, BLK, BLK},  // l
   {WHT, WHT, BLK},
   {BLK, WHT, BLK},
   {BLK, WHT, BLK},
   {BLK, WHT, BLK},
   {WHT, WHT, WHT} 
};

const uint16_t char_P[FONT_HEIGHT][FONT_WIDTH] = {
   {BLK, BLK, BLK},  // P
   {WHT, WHT, BLK},
   {WHT, BLK, WHT},
   {WHT, WHT, BLK},
   {WHT, BLK, BLK},
   {WHT, BLK, BLK} 
};

const uint16_t char_r[FONT_HEIGHT][FONT_WIDTH] = {
   {BLK, BLK, BLK},  // r
   {BLK, BLK, BLK},
   {BLK, WHT, WHT},
   {WHT, BLK, BLK},
   {WHT, BLK, BLK},
   {WHT, BLK, BLK} 
};

const uint16_t char_percent[FONT_HEIGHT][FONT_WIDTH] = {
   {BLK, BLK, BLK},  // %
   {BLK, BLK, WHT},
   {WHT, BLK, BLK},
   {BLK, WHT, BLK},
   {BLK, BLK, WHT},
   {WHT, BLK, BLK} 
};

const uint16_t char_point[FONT_HEIGHT][FONT_WIDTH] = {
   {BLK, BLK, BLK},  // .
   {BLK, BLK, BLK},
   {BLK, BLK, BLK},
   {BLK, BLK, BLK},
   {BLK, BLK, BLK},
   {BLK, WHT, BLK} 
};

/*=============================================================================
 =======                VARIABLES & MESSAGES & RESSOURCEN               =======
 =============================================================================*/
// timezone: Central European Time (Frankfurt, Paris)
TimeChangeRule daylight_time = { "DT", Last, Sun, Mar, 2, 120 };      //e.g. Central European Summer Time (CEST)
TimeChangeRule standard_time = { "ST", Last, Sun, Oct, 3, 60 };       //e.g. Central European Time (CET)
Timezone time_zone(daylight_time, standard_time);
TimeChangeRule *tcr;                                          //pointer to the time change rule, use to get the TZ abbrev

// Open Weather Map API
String owm_api_key;                                           // Open Weather Map API Key
String owm_city;                                              // Open Weather Map City
String owm_units;                                             // Open Weather Map Units, can be "imperial" or "metric"

// state machine
unsigned char state = 0;
boolean show_valid = false;

// to be displayed values
int temperature   = 9999;
int pressure      = 9999;
int humidity      = 9999;
char daynight_now = 'd';
char daynight_nxt = 'd';
char daynight_nxt_nxt = 'd';
int icon_id_now   = 0;
int icon_id_nxt   = 0;
int icon_id_nxt_nxt   = 0;

// wifi stuff
WiFiManager wifiManager;
WiFiUDP udp;

// time.nist.gov NTP server address, port to listen and packet buffer
IPAddress timeServerIP; 
String ntp_server;          // e.g. de.pool.ntp.org
unsigned int localPort = 2390;
byte packetBuffer[NTP_PACKET_SIZE];

// Set web server port number to 80
ESP8266WebServer server(80);

// Variable to store the HTTP request
String header;

// matrix display definition and temporary display buffer
PxMATRIX display(32,16, P_LAT, P_OE, P_A, P_B, P_C);
uint16_t display_buffer[DISPLAY_HEIGHT][DISPLAY_WIDTH] = {{BLK}};

// timer interrupts
Ticker display_ticker;
Ticker draw_ticker;
uint32_t ms100counter = 0ul;

// PIR
int pir_value = 0;
uint16_t pir_time;
uint16_t pir_count;

// Button
int button_value = 0;
uint8_t button_pressed = 0;

// Menu
uint8_t anim_line0_count = 0;
uint8_t menu = 0;

// Webpage
char webpage_buffer[WEBPAGE_BUFFER_SIZE];
String dt_week_selected[WEEK_SELECTIONS] = {"", "", "", "", ""};
String dt_dow_selected[DOW_SELECTIONS] = {"", "", "", "", "", "", ""};
String dt_month_selected[MONTH_SELECTIONS] = {"", "", "", "", "", "", "", "", "", "", "", ""};
String st_week_selected[WEEK_SELECTIONS] = {"", "", "", "", ""};
String st_dow_selected[DOW_SELECTIONS] = {"", "", "", "", "", "", ""};
String st_month_selected[MONTH_SELECTIONS] = {"", "", "", "", "", "", "", "", "", "", "", ""};
uint8_t dt_hour;
int     dt_offset;
uint8_t st_hour;
int     st_offset;

/*=============================================================================
 =======                              METHODS                           =======
 =============================================================================*/
uint16_t * getIcon(char daynight, int icon_number)
{
  int left, middle, right;
  
  left = 0;
  right = NBR_ICONS - 1;
  
  while (left <= right)
  {
    middle = (left + right) / 2;
   
    if (icon_number > icon_list[middle].icon_number)
    {
      left = middle + 1;
    }
   
    if (icon_number < icon_list[middle].icon_number)
    {
      right = middle - 1;
    }
   
    if(icon_number == icon_list[middle].icon_number)
    {
	  if (daynight == 'd')
	  {
        return (uint16_t *)icon_list[middle].icon_day;
	  }
	  else
	  {
        return (uint16_t *)icon_list[middle].icon_night;
	  }
    }
  }
  return NULL;
}

void draw_char(uint8_t x_pos, uint8_t y_pos, const uint16_t character[][FONT_WIDTH], uint8_t width, uint16_t color)
{
  uint8_t x, y;

  for (y = 1; y < FONT_HEIGHT; y ++)
  {
    for (x = 0; x < width; x ++)
    {
      display_buffer[y_pos + y - 1][x_pos + x] = character[y][x] & color;
    }
  }  
}

void draw_digit(uint8_t x_pos, uint8_t y_pos, uint8_t d, uint16_t color)
{
  uint8_t x, y;

  for (y = 1; y < FONT_HEIGHT; y ++)
  {
    for (x = 0; x < FONT_WIDTH; x ++)
    {
      display_buffer[y_pos + y - 1][x_pos + x] = digit[d][y][x] & color;
    }
  }  
}

// anim_step = 1, 2, 3, 4, 5, 6
void draw_digit_anim(uint8_t x_pos, uint8_t y_pos, uint8_t d_old, uint8_t d_new, uint8_t anim_step, uint16_t color)
{
  uint8_t x, y;

  for (y = 1; y < anim_step; y ++)
  {
    for (x = 0; x < FONT_WIDTH; x ++)
    {
      display_buffer[y_pos + y - 1][x_pos + x] = digit[d_new][FONT_HEIGHT - anim_step + y][x] & color;
    }
  }  
  for (y = anim_step; y < FONT_HEIGHT; y ++)
  {
    for (x = 0; x < FONT_WIDTH; x ++)
    {
      display_buffer[y_pos + y - 1][x_pos + x] = digit[d_old][y - anim_step][x] & color;
    }
  }  
}

void draw_weather_icon (uint8_t x_pos, uint8_t y_pos, char daynight, int icon_id)
{
  uint8_t x, y;
  uint16_t *icon = getIcon(daynight, icon_id);

  if (icon != NULL)
  {
    for (y = 0; y < ICON_HEIGHT; y ++)
    {
      for (x = 0; x < ICON_WIDTH; x ++)
      {
        display_buffer[y_pos + y][x_pos + x] = icon[y * ICON_WIDTH + x];
      }
    } 
  }
}

void draw_temperature(uint8_t x_pos, uint8_t y_pos, int temperature, uint16_t color)
{
  uint8_t x, y;
  uint8_t d, temp;

  if (temperature != 9999)
  {
    if (color == BLK)
    {
      if (temperature < -10)
      {
        color = display.color565(0, 0, 255);
      }
      if ((temperature >= -10) && (temperature < 0))
      {
        color = display.color565(51, 0, 204);
      }
      if ((temperature >= 0) && (temperature < 10))
      {
        color = display.color565(102, 0, 153);
      }
      if ((temperature >= 10) && (temperature < 20))
      {
        color = display.color565(153, 0, 102);
      }
      if ((temperature >= 20) && (temperature < 30))
      {
        color = display.color565(204, 0, 51);
      }
      if (temperature >= 30)
      {
        color = display.color565(255, 0, 0);
      }
    }
    
    for (y = y_pos; y < (y_pos + 5); y ++)
    {
      for (x = x_pos; x < (x_pos + 13); x ++)
      {
        display_buffer[y][x] = BLK;
      }
    }

    display_buffer[y_pos][x_pos + 11] = color;
    display_buffer[y_pos][x_pos + 12] = color;
    display_buffer[y_pos + 1][x_pos + 11] = color;
    display_buffer[y_pos + 1][x_pos + 12] = color;

    temp = (uint8_t)abs(temperature);
    
    d = temp % 10;
    draw_digit(x_pos + 7, y_pos, d, color);

    d = temp / 10;
    if (d > 0)
    {
      draw_digit(x_pos + 3, y_pos, d, color);
      if (temperature < 0)
      {
        display_buffer[y_pos + 2][x_pos]     = color;
        display_buffer[y_pos + 2][x_pos + 1] = color;
      }
    }
    else
    {
      if (temperature < 0)
      {
        display_buffer[y_pos + 2][x_pos + 3] = color;
        display_buffer[y_pos + 2][x_pos + 4] = color;
      }
    }  
  }
}

void draw_humidity(uint8_t x_pos, uint8_t y_pos, uint16_t color)
{
  uint8_t d;

  if (humidity != 9999)
  {
    d = (uint8_t)((humidity / 100) % 10);
    if (d != 0)
    {
      draw_digit(x_pos, y_pos, d, color);
      d = (uint8_t)((humidity / 10) % 10);
      draw_digit(x_pos + 4, y_pos, d, color);
    }
    else
    {
      d = (uint8_t)((humidity / 10) % 10);
      if (d != 0)
      {
        draw_digit(x_pos + 4, y_pos, d, color);
      }      
    }
      
    d = (uint8_t)(humidity % 10);
    draw_digit(x_pos + 8, y_pos, d, color);
  
    draw_char(x_pos + 13, y_pos, char_percent, 3, color);
    draw_char(x_pos + 17, y_pos, char_r, 3, color);
    draw_char(x_pos + 21, y_pos, char_e, 3, color);
    draw_char(x_pos + 25, y_pos, char_l, 3, color);
    draw_char(x_pos + 29, y_pos, char_point, 3, color);
  }
}

void draw_pressure(uint8_t x_pos, uint8_t y_pos, uint16_t color)
{
  uint8_t d;

  if (pressure != 9999)
  {
    d = (uint8_t)((pressure / 1000) % 10);
    if (d != 0)
    {
      draw_digit(x_pos, y_pos, d, color);
    }
    d = (uint8_t)((pressure / 100) % 10);
    draw_digit(x_pos + 4, y_pos, d, color); 
    d = (uint8_t)((pressure / 10) % 10);
    draw_digit(x_pos + 8, y_pos, d, color);
    d = (uint8_t)(pressure % 10);
    draw_digit(x_pos + 12, y_pos, d, color);
  
    draw_char(x_pos + 17, y_pos, char_h, 3, color);
    draw_char(x_pos + 21, y_pos, char_P, 3, color);
    draw_char(x_pos + 25, y_pos, char_a, 3, color);
  }
}

void draw_clock (uint8_t x_pos, uint8_t y_pos, uint16_t color)
{
  uint8_t d;
  static digit_anim_t clock_digit[6] = {{255, 0, 0, 0, 0, 0}, {255, 0, 0, 0, 0, 4}, {255, 0, 0, 0, 0, 10}, {255, 0, 0, 0, 0, 14}, {255, 0, 0, 0, 0, 20}, {255, 0, 0, 0, 0, 24}};	
  time_t local = time_zone.toLocal(now(), &tcr);
  uint8_t hours   = (uint8)hour(local);
  uint8_t minutes = (uint8)minute(local);
  uint8_t seconds = (uint8)second(local);
  
  clock_digit[0].digit_new = hours   / 10;
  clock_digit[1].digit_new = hours   % 10;
  clock_digit[2].digit_new = minutes / 10;
  clock_digit[3].digit_new = minutes % 10;
  clock_digit[4].digit_new = seconds / 10;
  clock_digit[5].digit_new = seconds % 10;

  for (d = 0; d < 6; d ++)
  {
	  if (clock_digit[d].digit_anim_step > 0)
	  {
	    clock_digit[d].digit_anim_step ++;
	    if (clock_digit[d].digit_anim_step <= FONT_HEIGHT)
	    {
        if (d < 4)
        {
		      draw_digit_anim(x_pos + clock_digit[d].digit_xoffset, y_pos, clock_digit[d].digit_show_old, clock_digit[d].digit_show_new, clock_digit[d].digit_anim_step, color);
        }
	    }
	    else
	    {
		    clock_digit[d].digit_show_old = clock_digit[d].digit_show_new;
		    clock_digit[d].digit_anim_step = 0;
        if (d==5)
        {
          display_buffer[y_pos + 1][x_pos + 8]  = BLK;
          display_buffer[y_pos + 3][x_pos + 8]  = BLK;
#ifdef CLOCK_DISPLAY_SECONDS
          display_buffer[y_pos + 1][x_pos + 18] = BLK;
          display_buffer[y_pos + 3][x_pos + 18] = BLK;
#endif
        }
	    }
	  }
	
	  if (clock_digit[d].digit_new != clock_digit[d].digit_old)
	  {
	    clock_digit[d].digit_show_new = clock_digit[d].digit_new;
	    clock_digit[d].digit_anim_step = 1;
      if ( d < 4)
      {
	      draw_digit_anim(x_pos + clock_digit[d].digit_xoffset, y_pos, clock_digit[d].digit_show_old, clock_digit[d].digit_show_new, clock_digit[d].digit_anim_step, color);
      }
      if (d == 5)
      {
        display_buffer[y_pos + 1][x_pos + 8]  = color;
        display_buffer[y_pos + 3][x_pos + 8]  = color;
#ifdef CLOCK_DISPLAY_SECONDS
        display_buffer[y_pos + 1][x_pos + 18] = color;
        display_buffer[y_pos + 3][x_pos + 18] = color;
#endif
      }
	  }

    clock_digit[d].digit_old = clock_digit[d].digit_new;	
  }
}

// called with 32 .. 1
void draw_anim_line0(uint8 step)
{
  uint8_t x, y;

  for (y = 0; y < LINE0_END; y ++)
  {
    for (x = 0; x < (step - 1); x ++)
    {
      display_buffer[y][x] = display_buffer[y][x + 1];
    }
    display_buffer[y][step - 1] = BLK;
  }   
}

void draw_display_buffer(void)
{
  uint8_t x, y;

  for (y = 0; y < DISPLAY_HEIGHT; y ++)
  {
    for (x = 0; x < DISPLAY_WIDTH; x ++)
    {
      if (display_buffer[y][x] > BLK)
      {
        display.drawPixel(x, y, display_buffer[y][x]);
      }
    }
  }
}

// ISR for display refresh (every 2ms)
void display_updater()
{
  display.display(200);
}

// ISR for drawing update (every 100ms)
void draw_updater()
{
  display.clearDisplay();
  
  if (pir_count > 0)
  {
#ifndef DISPLAY_ON_PERMANENTLY
    pir_count --;
#endif
  }

  if ((button_pressed == 1) && (anim_line0_count == 0))
  {
    anim_line0_count = DISPLAY_WIDTH + 1;
  }

  if (anim_line0_count > 0)
  {
    anim_line0_count --;
    if (anim_line0_count == 0)
    {
      menu = (menu + 1) % NBR_MENUS;
    }
  }
  
  if ((show_valid == true) && (pir_count > 0))
  {
    draw_clock(0, 10, ORG);
    draw_temperature(19, 10, temperature, BLK);
    if (anim_line0_count == 0)
    {
      switch (menu)
      {
        case 0: // draw weather icons and temperature
          draw_weather_icon(0,  0, daynight_now, icon_id_now);   
          draw_weather_icon(11, 0, daynight_nxt, icon_id_nxt);    
          draw_weather_icon(22, 0, daynight_nxt_nxt, icon_id_nxt_nxt);    
          break;
        case 1: // draw humidity
          draw_humidity(0, 2, BLU);
          break;
        case 2: // draw pressure
          draw_pressure(2, 2, GRN);
          break;
        default:
          break;        
      }
    }
    else
    {
      draw_anim_line0(anim_line0_count);
    }
    draw_display_buffer(); 
  }  
  ms100counter++;
}

void getCurrentWeather(void)
{
  // determined capacity by https://arduinojson.org/v5/assistant/
  const size_t capacity = JSON_ARRAY_SIZE(2) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(2) + 2*JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(12) + 440;
  DynamicJsonBuffer jsonBuffer(capacity);
  HTTPClient http;

  http.begin("http://api.openweathermap.org/data/2.5/weather?q="+owm_city+"&appid="+owm_api_key+"&units="+owm_units);
  if (http.GET() == HTTP_CODE_OK)
  {
    String payload = http.getString();
    JsonObject& owm_data = jsonBuffer.parseObject(payload);
    if (!owm_data.success())
    {
      Serial.printf("Parsing failed\n");
    }
    else
    {
      String icon = owm_data["weather"][0]["icon"];
      daynight_now =  icon.charAt(2);
      icon_id_now = owm_data["weather"][0]["id"];
      temperature = ROUND_INT(((float)owm_data["main"]["temp"]));
      pressure = owm_data["main"]["pressure"];
      humidity = owm_data["main"]["humidity"];
      Serial.printf("Now: %c %d %d %d %d\n", daynight_now, icon_id_now, temperature, pressure, humidity);
    }
  }
  http.end();
}

void getForecastWeather(void)
{
  // determined capacity by https://arduinojson.org/v5/assistant/
  const size_t capacity = 2*JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(2) + 6*JSON_OBJECT_SIZE(1) + 3*JSON_OBJECT_SIZE(2) + 2*JSON_OBJECT_SIZE(4) + 2*JSON_OBJECT_SIZE(5) + 4*JSON_OBJECT_SIZE(8) + 760;
  DynamicJsonBuffer jsonBuffer(capacity);
  HTTPClient http;

  http.begin("http://api.openweathermap.org/data/2.5/forecast?q="+owm_city+"&appid="+owm_api_key+"&units="+owm_units+"&cnt=2");
  if (http.GET() == HTTP_CODE_OK)
  {
    String payload = http.getString();
    JsonObject& owm_data = jsonBuffer.parseObject(payload);

    if (!owm_data.success())
    {
      Serial.printf("Parsing failed\n");
    }
    else
    {
      String icon  = owm_data["list"][0]["weather"][0]["icon"];
      String icon2 = owm_data["list"][1]["weather"][0]["icon"];
      daynight_nxt =  icon.charAt(2);
      icon_id_nxt = owm_data["list"][0]["weather"][0]["id"];      
      daynight_nxt_nxt =  icon2.charAt(2);
      icon_id_nxt_nxt = owm_data["list"][1]["weather"][0]["id"];
      Serial.printf("Nxt: %c %d NxtNxt: %c %d\n", daynight_nxt, icon_id_nxt, daynight_nxt_nxt, icon_id_nxt_nxt);
    }
  }
  http.end();
}

// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}

void handleRootPath(void)            //Handler for the root path
{
  int a, i;
  
  for (a = 0; a < server.args(); a++) 
  { 
    if (server.argName(a).equals("owm_api_key") == true)
    {
      char owm_api_key_temp[OWM_API_KEY_LEN];
      
      owm_api_key = server.arg(a);
      memcpy (owm_api_key_temp, owm_api_key.c_str(), OWM_API_KEY_LEN);  
      for (i = 0; i < OWM_API_KEY_LEN; i ++)
      {
        EEPROM.write(OWM_API_KEY_OFFSET + i, owm_api_key_temp[i]);
      }
    }
    
    if (server.argName(a).equals("owm_city") == true)
    {
      char owm_city_temp[OWM_CITY_LEN];
      
      owm_city = server.arg(a);
      memcpy (owm_city_temp, owm_city.c_str(), OWM_CITY_LEN);  
      for (i = 0; i < OWM_CITY_LEN; i ++)
      {
        EEPROM.write(OWM_CITY_OFFSET + i, owm_city_temp[i]);
      }
    }
    
    if (server.argName(a).equals("owm_units") == true)
    {
      char owm_units_temp[OWM_UNITS_LEN];
      
      owm_units = server.arg(a);
      memcpy (owm_units_temp, owm_units.c_str(), OWM_UNITS_LEN);  
      for (i = 0; i < OWM_UNITS_LEN; i ++)
      {
        EEPROM.write(OWM_UNITS_OFFSET + i, owm_units_temp[i]);
      }
    }
    
    if (server.argName(a).equals("ntp_server") == true)
    {
      char ntp_server_temp[NTP_SERVER_LEN];
      
      ntp_server = server.arg(a);
      memcpy (ntp_server_temp, ntp_server.c_str(), NTP_SERVER_LEN);  
      for (i = 0; i < NTP_SERVER_LEN; i ++)
      {
        EEPROM.write(NTP_SERVER_OFFSET + i, ntp_server_temp[i]);
      }
    }

    if (server.argName(a).equals("dt_week") == true)
    {      
      uint8_t dt_week_temp = (uint8_t)server.arg(a).toInt();
      if (dt_week_temp >= WEEK_SELECTIONS)
      {
        dt_week_temp = Last;
      }
      for (i = 0; i < WEEK_SELECTIONS; i ++)
      {
        dt_week_selected[i] = "";
      }
      dt_week_selected[dt_week_temp] = " selected";
      EEPROM.write(DT_WEEK_OFFSET, dt_week_temp);
    }

    if (server.argName(a).equals("dt_dow") == true)
    {      
      uint8_t dt_dow_temp = (uint8_t)server.arg(a).toInt();
      if ((dt_dow_temp > DOW_SELECTIONS) || (dt_dow_temp == 0))
      {
        dt_dow_temp = Sun;
      }
      for (i = 0; i < DOW_SELECTIONS; i ++)
      {
        dt_dow_selected[i] = "";
      }
      dt_dow_selected[dt_dow_temp - 1] = " selected";
      EEPROM.write(DT_DOW_OFFSET, dt_dow_temp);
    }

    if (server.argName(a).equals("dt_month") == true)
    {      
      uint8_t dt_month_temp = (uint8_t)server.arg(a).toInt();
      if ((dt_month_temp > MONTH_SELECTIONS) || (dt_month_temp == 0))
      {
        dt_month_temp = Mar;
      }
      for (i = 0; i < MONTH_SELECTIONS; i ++)
      {
        dt_month_selected[i] = "";
      }
      dt_month_selected[dt_month_temp - 1] = " selected";
      EEPROM.write(DT_MONTH_OFFSET, dt_month_temp);
    }

    if (server.argName(a).equals("dt_hour") == true)
    {
      dt_hour = (uint8_t)server.arg(a).toInt();
      if (dt_hour > 23)
      {
        dt_hour = 2;
      }
      EEPROM.write(DT_HOUR_OFFSET, dt_hour);
    }

    if (server.argName(a).equals("dt_offset") == true)
    {
      uint32_t dt_offset_temp;
      
      dt_offset = server.arg(a).toInt();
      if ((dt_offset > 1440) || (dt_offset < -1440))
      {
        dt_offset = 120;
      }
      dt_offset_temp = (uint32_t)dt_offset;
      EEPROM.write(DT_OFFSET_OFFSET,     (uint8_t)dt_offset_temp);
      EEPROM.write(DT_OFFSET_OFFSET + 1, (uint8_t)(dt_offset_temp >> 8));
      EEPROM.write(DT_OFFSET_OFFSET + 2, (uint8_t)(dt_offset_temp >> 16));
      EEPROM.write(DT_OFFSET_OFFSET + 3, (uint8_t)(dt_offset_temp >> 24));
    }

    if (server.argName(a).equals("st_week") == true)
    {      
      uint8_t st_week_temp = (uint8_t)server.arg(a).toInt();
      if (st_week_temp >= WEEK_SELECTIONS)
      {
        st_week_temp = Last;
      }
      for (i = 0; i < WEEK_SELECTIONS; i ++)
      {
        st_week_selected[i] = "";
      }
      st_week_selected[st_week_temp] = " selected";
      EEPROM.write(ST_WEEK_OFFSET, st_week_temp);
    }

    if (server.argName(a).equals("st_dow") == true)
    {      
      uint8_t st_dow_temp = (uint8_t)server.arg(a).toInt();
      if ((st_dow_temp > DOW_SELECTIONS) || (st_dow_temp == 0))
      {
        st_dow_temp = Sun;
      }
      for (i = 0; i < DOW_SELECTIONS; i ++)
      {
        st_dow_selected[i] = "";
      }
      st_dow_selected[st_dow_temp - 1] = " selected";
      EEPROM.write(ST_DOW_OFFSET, st_dow_temp);
    }

    if (server.argName(a).equals("st_month") == true)
    {      
      uint8_t st_month_temp = (uint8_t)server.arg(a).toInt();
      if ((st_month_temp > MONTH_SELECTIONS) || (st_month_temp == 0))
      {
        st_month_temp = Oct;
      }
      for (i = 0; i < MONTH_SELECTIONS; i ++)
      {
        st_month_selected[i] = "";
      }
      st_month_selected[st_month_temp - 1] = " selected";
      EEPROM.write(ST_MONTH_OFFSET, st_month_temp);
    }

    if (server.argName(a).equals("st_hour") == true)
    {
      st_hour = (uint8_t)server.arg(a).toInt();
      if (st_hour > 23)
      {
        st_hour = 3;
      }
      EEPROM.write(ST_HOUR_OFFSET, st_hour);
    }

    if (server.argName(a).equals("st_offset") == true)
    {
      uint32_t st_offset_temp;
      
      st_offset = server.arg(a).toInt();
      if ((st_offset > 1440) || (st_offset < -1440))
      {
        st_offset = 60;
      }
      st_offset_temp = (uint32_t)st_offset;
      EEPROM.write(ST_OFFSET_OFFSET,     (uint8_t)st_offset_temp);
      EEPROM.write(ST_OFFSET_OFFSET + 1, (uint8_t)(st_offset_temp >> 8));
      EEPROM.write(ST_OFFSET_OFFSET + 2, (uint8_t)(st_offset_temp >> 16));
      EEPROM.write(ST_OFFSET_OFFSET + 3, (uint8_t)(st_offset_temp >> 24));
    }

    if (server.argName(a).equals("pir_time") == true)
    {
      pir_time = (uint16_t)server.arg(a).toInt();
      if (pir_time > PIR_TIME_MAX)
      {
        pir_time = 600; // default 10 minutes
      }
      EEPROM.write(PIR_TIME_OFFSET,     (uint8_t)pir_time);
      EEPROM.write(PIR_TIME_OFFSET + 1, (uint8_t)(pir_time >> 8));
    }
  } 

  EEPROM.commit();

  snprintf(webpage_buffer, WEBPAGE_BUFFER_SIZE,
  "<html>\
    <body>\
      <h1>Matrix Weather Clock</h1>\
       <form>\
        Open Weather Map API Key: <input type=\"text\" name=\"owm_api_key\" size=\"%d\" value=\"%s\"><br>\
        Open Weather Map City: <input type=\"text\" name=\"owm_city\" size=\"%d\" value=\"%s\"><br>\
        Open Weather Map Units: <input type=\"text\" name=\"owm_units\" size=\"%d\" value=\"%s\"> (imperial, metric)<br>\
        NTP Server Address: <input type=\"text\" name=\"ntp_server\" size=\"%d\" value=\"%s\"><br>\
        Daylight Saving Time:\
        week - <select name=\"dt_week\">\
          <option value=\"0\"%s>Last</option>\
          <option value=\"1\"%s>First</option>\
          <option value=\"2\"%s>Second</option>\
          <option value=\"3\"%s>Third</option>\
          <option value=\"4\"%s>Fourth</option>\
        </select>\
        day - <select name=\"dt_dow\">\
          <option value=\"1\"%s>Sun</option>\
          <option value=\"2\"%s>Mon</option>\
          <option value=\"3\"%s>Tue</option>\
          <option value=\"4\"%s>Wed</option>\
          <option value=\"5\"%s>Thu</option>\
          <option value=\"6\"%s>Fri</option>\
          <option value=\"7\"%s>Sat</option>\
        </select>\
        month - <select name=\"dt_month\">\
          <option value=\"1\"%s>Jan</option>\
          <option value=\"2\"%s>Feb</option>\
          <option value=\"3\"%s>Mar</option>\
          <option value=\"4\"%s>Apr</option>\
          <option value=\"5\"%s>May</option>\
          <option value=\"6\"%s>Jun</option>\
          <option value=\"7\"%s>Jul</option>\
          <option value=\"8\"%s>Aug</option>\
          <option value=\"9\"%s>Sep</option>\
          <option value=\"10\"%s>Oct</option>\
          <option value=\"11\"%s>Nov</option>\
          <option value=\"12\"%s>Dec</option>\
        </select>\
        hour - <input type=\"text\" name=\"dt_hour\" size=\"2\" value=\"%d\">\
        offset - <input type=\"text\" name=\"dt_offset\" size=\"5\" value=\"%d\"> (min)<br>\
        Standard Time:\
        week - <select name=\"st_week\">\
          <option value=\"0\"%s>Last</option>\
          <option value=\"1\"%s>First</option>\
          <option value=\"2\"%s>Second</option>\
          <option value=\"3\"%s>Third</option>\
          <option value=\"4\"%s>Fourth</option>\
        </select>\
        day - <select name=\"st_dow\">\
          <option value=\"1\"%s>Sun</option>\
          <option value=\"2\"%s>Mon</option>\
          <option value=\"3\"%s>Tue</option>\
          <option value=\"4\"%s>Wed</option>\
          <option value=\"5\"%s>Thu</option>\
          <option value=\"6\"%s>Fri</option>\
          <option value=\"7\"%s>Sat</option>\
        </select>\
        month - <select name=\"st_month\">\
          <option value=\"1\"%s>Jan</option>\
          <option value=\"2\"%s>Feb</option>\
          <option value=\"3\"%s>Mar</option>\
          <option value=\"4\"%s>Apr</option>\
          <option value=\"5\"%s>May</option>\
          <option value=\"6\"%s>Jun</option>\
          <option value=\"7\"%s>Jul</option>\
          <option value=\"8\"%s>Aug</option>\
          <option value=\"9\"%s>Sep</option>\
          <option value=\"10\"%s>Oct</option>\
          <option value=\"11\"%s>Nov</option>\
          <option value=\"12\"%s>Dec</option>\
        </select>\
        hour - <input type=\"text\" name=\"st_hour\" size=\"2\" value=\"%d\">\
        offset - <input type=\"text\" name=\"st_offset\" size=\"5\" value=\"%d\"> (min)<br>\
        Motion Detection Hold Time: <input type=\"text\" name=\"pir_time\" size=\"4\" value=\"%d\"> (sec)<br>\
        <input type=\"submit\" value=\"Save\">\
       </form>\
       <p>Click the \"Save\" button and the data will be saved to EEPROM.</p>\
    </body>\
  </html>", OWM_API_KEY_LEN, owm_api_key.c_str(), OWM_CITY_LEN, owm_city.c_str(), OWM_UNITS_LEN, owm_units.c_str(), NTP_SERVER_LEN, ntp_server.c_str(),
            dt_week_selected[0].c_str(), dt_week_selected[1].c_str(), dt_week_selected[2].c_str(), dt_week_selected[3].c_str(), dt_week_selected[4].c_str(),
            dt_dow_selected[0].c_str(), dt_dow_selected[1].c_str(), dt_dow_selected[2].c_str(), dt_dow_selected[3].c_str(), dt_dow_selected[4].c_str(), dt_dow_selected[5].c_str(), dt_dow_selected[6].c_str(),
            dt_month_selected[0].c_str(), dt_month_selected[1].c_str(), dt_month_selected[2].c_str(), dt_month_selected[3].c_str(), dt_month_selected[4].c_str(), dt_month_selected[5].c_str(), dt_month_selected[6].c_str(), dt_month_selected[7].c_str(), dt_month_selected[8].c_str(), dt_month_selected[9].c_str(), dt_month_selected[10].c_str(), dt_month_selected[11].c_str(),
            dt_hour, dt_offset,
            st_week_selected[0].c_str(), st_week_selected[1].c_str(), st_week_selected[2].c_str(), st_week_selected[3].c_str(), st_week_selected[4].c_str(),
            st_dow_selected[0].c_str(), st_dow_selected[1].c_str(), st_dow_selected[2].c_str(), st_dow_selected[3].c_str(), st_dow_selected[4].c_str(), st_dow_selected[5].c_str(), st_dow_selected[6].c_str(),
            st_month_selected[0].c_str(), st_month_selected[1].c_str(), st_month_selected[2].c_str(), st_month_selected[3].c_str(), st_month_selected[4].c_str(), st_month_selected[5].c_str(), st_month_selected[6].c_str(), st_month_selected[7].c_str(), st_month_selected[8].c_str(), st_month_selected[9].c_str(), st_month_selected[10].c_str(), st_month_selected[11].c_str(),
            st_hour, st_offset,
            pir_time);
  
  server.send(200, "text/html", webpage_buffer); 
}

void readEEPROM(void)
{
  int i;
  bool erased;
  uint8_t owm_api_key_temp[OWM_API_KEY_LEN + 1]; 
  uint8_t owm_city_temp[OWM_CITY_LEN + 1];
  uint8_t owm_units_temp[OWM_UNITS_LEN + 1]; 
  uint8_t ntp_server_temp[NTP_SERVER_LEN + 1];
  uint8_t dt_offset_temp[DT_OFFSET_LEN]; 
  uint8_t st_offset_temp[ST_OFFSET_LEN]; 
  uint8_t pir_time_temp[PIR_TIME_LEN];

  erased = true;
  for (i = 0; i < OWM_API_KEY_LEN; i++)
  {
    owm_api_key_temp[i] = EEPROM.read(OWM_API_KEY_OFFSET + i);
    if (owm_api_key_temp[i] != 0xff)
    {
      erased = false;
    }
  }
  owm_api_key_temp[OWM_API_KEY_LEN] = 0;
  if (erased == false)
  {
    owm_api_key = (char*)owm_api_key_temp; 
  }
  else
  {
    owm_api_key = "";
  }
  
  erased = true;
  for (i = 0; i < OWM_CITY_LEN; i++)
  {
    owm_city_temp[i] = EEPROM.read(OWM_CITY_OFFSET + i);
    if (owm_city_temp[i] != 0xff)
    {
      erased = false;
    }
  }
  owm_city_temp[OWM_CITY_LEN] = 0;
  if (erased == false)
  {
    owm_city = (char*)owm_city_temp; 
  }
  else
  {
    owm_city = "";
  }

  erased = true;
  for (i = 0; i < OWM_UNITS_LEN; i++)
  {
    owm_units_temp[i] = EEPROM.read(OWM_UNITS_OFFSET + i);
    if (owm_units_temp[i] != 0xff)
    {
      erased = false;
    }
  }
  owm_units_temp[OWM_UNITS_LEN] = 0;
  if (erased == false)
  {
    owm_units = (char*)owm_units_temp; 
  }
  else
  {
    owm_units = "metric";
  }

  erased = true;
  for (i = 0; i < NTP_SERVER_LEN; i++)
  {
    ntp_server_temp[i] = EEPROM.read(NTP_SERVER_OFFSET + i);
    if (ntp_server_temp[i] != 0xff)
    {
      erased = false;
    }
  }
  ntp_server_temp[NTP_SERVER_LEN] = 0;
  if (erased == false)
  {
    ntp_server = (char*)ntp_server_temp; 
  }
  else
  {
    ntp_server = "pool.ntp.org";
  }

  daylight_time.week = EEPROM.read(DT_WEEK_OFFSET);
  if (daylight_time.week >= WEEK_SELECTIONS)
  {
    daylight_time.week = Last; // CEST
  }
  dt_week_selected[daylight_time.week] = " selected";

  daylight_time.dow = EEPROM.read(DT_DOW_OFFSET);
  if ((daylight_time.dow > DOW_SELECTIONS) || (daylight_time.dow == 0))
  {
    daylight_time.dow = Sun; // CEST
  }
  dt_dow_selected[daylight_time.dow - 1] = " selected";

  daylight_time.month = EEPROM.read(DT_MONTH_OFFSET);
  if ((daylight_time.month > MONTH_SELECTIONS) || (daylight_time.month == 0))
  {
    daylight_time.month = Mar; // CEST
  }
  dt_month_selected[daylight_time.month - 1] = " selected";

  daylight_time.hour = EEPROM.read(DT_HOUR_OFFSET);
  if (daylight_time.hour > 23)
  {
    daylight_time.hour = 2; // CEST
  }
  dt_hour = daylight_time.hour;

  dt_offset_temp[0] = EEPROM.read(DT_OFFSET_OFFSET);
  dt_offset_temp[1] = EEPROM.read(DT_OFFSET_OFFSET + 1);
  dt_offset_temp[2] = EEPROM.read(DT_OFFSET_OFFSET + 2);
  dt_offset_temp[3] = EEPROM.read(DT_OFFSET_OFFSET + 3);
  daylight_time.offset = (int)((((uint32_t)dt_offset_temp[3]) << 24) | (((uint32_t)dt_offset_temp[2]) << 16) | (((uint32_t)dt_offset_temp[1]) << 8) | (((uint32_t)dt_offset_temp[0]) << 0));
  if ((daylight_time.offset > 1440) || (daylight_time.offset < -1440))
  {
    daylight_time.offset = 120; // CEST
  }
  dt_offset = daylight_time.offset;

  standard_time.week = EEPROM.read(ST_WEEK_OFFSET);
  if (standard_time.week >= WEEK_SELECTIONS)
  {
    standard_time.week = Last; // CET
  }
  st_week_selected[standard_time.week] = " selected";

  standard_time.dow = EEPROM.read(ST_DOW_OFFSET);
  if ((standard_time.dow > DOW_SELECTIONS) || (standard_time.dow == 0))
  {
    standard_time.dow = Sun; // CET
  }
  st_dow_selected[standard_time.dow - 1] = " selected";

  standard_time.month = EEPROM.read(ST_MONTH_OFFSET);
  if ((standard_time.month > MONTH_SELECTIONS) || (standard_time.month == 0))
  {
    standard_time.month = Oct; // CET
  }
  st_month_selected[standard_time.month - 1] = " selected";

  standard_time.hour = EEPROM.read(ST_HOUR_OFFSET);
  if (standard_time.hour > 23)
  {
    standard_time.hour = 3; // CET
  }
  st_hour = standard_time.hour;

  st_offset_temp[0] = EEPROM.read(ST_OFFSET_OFFSET);
  st_offset_temp[1] = EEPROM.read(ST_OFFSET_OFFSET + 1);
  st_offset_temp[2] = EEPROM.read(ST_OFFSET_OFFSET + 2);
  st_offset_temp[3] = EEPROM.read(ST_OFFSET_OFFSET + 3);
  standard_time.offset = (int)((((uint32_t)st_offset_temp[3]) << 24) | (((uint32_t)st_offset_temp[2]) << 16) | (((uint32_t)st_offset_temp[1]) << 8) | (((uint32_t)st_offset_temp[0]) << 0));
  if ((standard_time.offset > 1440) || (standard_time.offset < -1440))
  {
    standard_time.offset = 60; // CET
  }
  st_offset = standard_time.offset;

  pir_time_temp[0] = EEPROM.read(PIR_TIME_OFFSET);
  pir_time_temp[1] = EEPROM.read(PIR_TIME_OFFSET + 1);
  pir_time = ((((uint16_t)pir_time_temp[1]) << 8) | (((uint16_t)pir_time_temp[0]) << 0));
  if (pir_time > PIR_TIME_MAX)
  {
    pir_time = 600;  // default 10 minutes
  }  
  pir_count = pir_time * 10;
}

void setup() 
{    
  Serial.begin(115200);
  
  // display layout 1/8 step
  display.begin(8);

  // Button (must be Pullup input, D3 = GPIO0)
  pinMode(D3, INPUT_PULLUP);

  // PIR sensor (must be Hi-Z input, D6 = GPIO12)
  pinMode(D6, INPUT);

  //display.setFastUpdate(false);
  display.clearDisplay();

  // set custom ip for portal
  wifiManager.setAPStaticIPConfig(IPAddress(192,168,1,1), IPAddress(192,168,1,1), IPAddress(255,255,255,0));
  
  wifiManager.setTimeout(AP_TIMEOUT); // Timeout until config portal is turned off
  if (!wifiManager.autoConnect(AP_NAME))
  {
    Serial.printf("Failed to connect and hit timeout\n");
    delay(3000);
    //reset and try again
    ESP.restart();
  }

  EEPROM.begin(128);
  
  readEEPROM();

  time_zone.setRules(daylight_time, standard_time);

  udp.begin(localPort);
  
  display_ticker.attach(0.002, display_updater);
  draw_ticker.attach(0.1, draw_updater);
 
  server.on("/", handleRootPath);    //Associate the handler function to the path
  server.on("/resetWifi", []() {
    server.send(200, "text/plain", "resetting Wifi ...");
    Serial.printf("resetting Wifi ...\n");
    delay(1000);
    wifiManager.resetSettings();
    delay(1000);
    //reset
    ESP.restart();
  });

  server.begin();                    //Start the server

  yield();
}

void loop() 
{
  server.handleClient();         //Handling of incoming requests
  
  pir_value = digitalRead(D6);
  if (pir_value != 0)
  {
    pir_count = pir_time * 10;
  }

  button_value = digitalRead(D3);
  if (button_value != 0)
  {
    button_pressed = 0;
  }
  else
  {
    button_pressed = 1;
  }
  
  switch (state)
  {
    case 0:     
      if (WiFi.status() == WL_CONNECTED)
      {
        WiFi.hostByName(ntp_server.c_str(), timeServerIP); //get a random server from the pool
        sendNTPpacket(timeServerIP); // send an NTP packet to a time server
        ms100counter = 0;
        state = 1;
      }
      break;
    case 1:
      if (ms100counter >= 10) // wait at least 1 sec for ntp response
      {
        ms100counter = 0;
        state = 2;
      }
      break;
    case 2:
      if (WiFi.status() == WL_CONNECTED)
      {
        if (NTP_PACKET_SIZE <= udp.parsePacket())
        {
          unsigned long secsSince1900;
          unsigned long epoch;
        
          // We've received a packet, read the data from it
          udp.read(packetBuffer, NTP_PACKET_SIZE);
    
          //the timestamp starts at byte 40 of the received packet and is four bytes,
          secsSince1900  = (unsigned long)packetBuffer[40] << 24;
          secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
          secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
          secsSince1900 |= (unsigned long)packetBuffer[43];
      
          // now convert NTP time into everyday time.
          // Unix time starts on Jan 1 1970. In seconds, that's 2208988800.
          // subtract seventy years.
          epoch = secsSince1900 - SEVENTYYEARS;
          setTime(epoch);
        
          // print Unix time
          Serial.print("Unix time = ");
          Serial.println(epoch);
        }
        ms100counter = 0;
        state = 3;
      }
      else
      {
        state = 0;
      }
      break; 
    case 3:
      if (ms100counter >= 50)  // wait 5 seconds
      {
        ms100counter = 0;
        state = 4;
      }
      break;
    case 4:
      getCurrentWeather();
      ms100counter = 0;
      state = 5;
      break;
    case 5:
      if (ms100counter >= 50) // wait 5 seconds
      {
        ms100counter = 0;
        state = 6;
      }
      break;
    case 6:
      getForecastWeather();
      ms100counter = 0;
      state = 7;
      show_valid = true;
      break;
    case 7:
      if (ms100counter >= UPDATE_PERIOD) // wait 10 minutes
      {
        ms100counter = 0;
        state = 0;
      }
      break;
    default:
      break;
  }
  
  yield();
}

