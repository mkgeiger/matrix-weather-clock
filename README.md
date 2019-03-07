# LED matrix clock with weather informations

## Overview

After my old 7-segmented clock in the living room gave up, I decided to build my own really fancy eye catching clock. The decicion fell on a LED matrix clock (after a Nixie clock was ruled out because of its limited capabilities and relative short tube lifes) because of its 80s retro look. The feature for my needs are:

* the device shall only communicate over WIFI to the outside world
* configurable WIFI access parameters
* the clock shall be using NTP to adjust its time automatically (I am always tired of setting the time after a power failure or when there is a change to the daylight saving time)
* the color of the time shall be configurable and also the seconds to be displayed or not
* a flashing colon (1/s) and a scrolling of the digits (from top to down) for the retro look
* displaying permanently the ouside temperature with a color gradient from blue (cold) to red (hot)
* displaying the actual weather icon and 2 weather forecast icons in the menu row, or alternativly the humidity or the air pressure
* changing the menu by pressing the push button shall have a retro scrolling effect
* the OpenWeatherMap API shall be used for requesting all weather information (I know its sometimes less accurate then commmercial ones, but its free!)
* all individual settings like time zone (incl. daylight saving and nornal time), OpenWeatherMap API key, OpenWeatherMap API city, OpenWeatherMap API units, NTP server address and motion detection hold time shall be configurable with a WEB server providing a settings page.
* a PIR (Pyroelectric Infrared Sensor) shall turn on the display when motion is detected and turn off the display again after a configurable time

## Mechanics

The different layers
![Layers](/mechanics/grid_foil_glas.jpg)

### Dividing grid

You need a dividing grid in order to prevent neighbour pixels from blending colors. Without a dividing grid, there will be no sharp edges between two pixels, but a gradient, making the resulting image look blurry. The grid I designed is for a LED matrix display 32x16 RGB P6, means for 6mm led raster. The STL-file (an abbreviation of "stereolithography") created with FreeCad can be downloaded [here](/mechanics/grid.stl). The STL-file can be sent to any online 3D-Print service.

### Diffuser

It is very important to take the thinnest foil but with the most diffused effect at the same time. Don't take diffused acrylic glas, this is too thick and too blury. Plastic sheets from crafting suppliers are also most times too blury. Good results you get with adhesive foils for windows for privacy. I ordered my foils at [Velken](www.velken.de) (item no. 213010 or 213001). Glue instructions from Velken are included.

### Glas pane

Online offers for individual sizes of panes you get at Ebay. A 2mm thick pane is enough. The diffuser foil is glued onto the glas pane. 

## Hardware

### Display

![Display](/hardware/display.jpg)

This is a LED matrix display which has in total 512 (32x16) RGB LEDs. If you want to learn more about the internal electronic details of such a display, please visit this page: https://bikerglen.com/projects/lighting/led-panel-1up/#The_LED_Panel. The setup and cabling for usage with the PxMatrix driver you should visit the page: https://github.com/2dom/PxMatrix/blob/master/README.md and https://learn.adafruit.com/32x16-32x32-rgb-led-matrix/new-wiring. For powering please visit this page: https://learn.adafruit.com/32x16-32x32-rgb-led-matrix/powering. The main power supply for the display should deliver at least a current of 2A. I had to power the display with about 4.3 Volt, maybe because the logic of display does not detect High-levels (3.3 Volt) from the ESP8266 correctly when powered with 5 Volt.

![Display](/hardware/display_pcb.jpg)

### NodeMcu ESP8266-12E

I decided to take this microcontroller board because of its power, easy programming, WIFI and small form factor. Following GPIOs are used:

  Signal    | ESP8266 | NodeMcu
  ----------|---------|--------
  MOSI (R0) | GPIO13  | D7
  A         | GPIO5   | D1
  B         | GPIO4   | D2
  C         | GPIO15  | D8
  CLK       | GPIO14  | D5
  OE        | GPIO2   | D4
  STB       | GPIO16  | D0
  PIR       | GPIO12  | D6
  BUTTON    | GPIO0   | D3
  GND       | GND     | GND
  
This module can be obtained at Ebay for about 3€.  
  
![NodeMcu](/hardware/NodeMcu_ESP8266_12E.png)  

### PIR sensor

![NodeMcu](/hardware/hc_sr505.jpg) 

The PIR sensor is used to switch off the display after some configurable time (default 10 min), when no motion is detected. This is mainly for extending the lifetime of the display LEDs. I have choosen the embedded PIR sensor HC-SR505, which is one of the smallest and cheapest. I had to add a suppression choke to filter high frequency signals like WIFI from he power lines, otherwise I got false positives and the display never turned off. The sensing distance for me is also very poor (only 2m) and not 3m as in the datasheet. All in all I have deaktivated the "display switching off" in mean time (DISPLAY_ON_PERMANENTLY), until the problem with the distance is solved. 2 ways to solve: 1. increasing the amplifying factor of the HC-SR505 OPAMP or 2. using the radar sensor [RWCL-0516](/hardware/rwcl_0516.jpg). I tend to solution 2 but no idea of the permanent microwave emission influencing the healt.

## Software

### Preparations

First install the [Arduino IDE](https://www.arduino.cc/en/main/software). Under board administration install support for "NodeMCU 1.0 (ESP-12E Module)". The CPU frequency has to be configured for "160 MHz". Following libraries have to be installed:

* PxMatrix
* Time
* Timezone
* ArduinoJson
* WiFi
* WiFiManager
* DNSServer
* ESP8266WebServer
* ESP8266HTTPClient

### NTP client

The NTP client adjusts the internal clock every 10 minutes. The timestamp (UTC Unix time) is converted to the local timezone considering the standard or daylight saving time. An individual NTP server (default: de.pool.ntp.org) can be configured over the WEB interface.

### Time Zone

![Timezone](/photos/timezone.png)

The time zone can be configured over the WEB interface. If the daylight saving time is not applicable to your time zone set both standard and dylight saving times the same values.

### OpenWeatherMap API

* first request an account at [OpenWeatherMap](https://home.openweathermap.org/users/sign_up) to obtain an individual API key.
* test following URLs in your browser and fill in your city and API key: [query1](http://api.openweathermap.org/data/2.5/weather?q=<city>&appid=<apikey>&units=metric) and [query2](http://api.openweathermap.org/data/2.5/forecast?q=<city>&appid=<apikey>&units=metric&cnt=2)
* when you get back the weather information in JSON format, everything is ok. You can now enter the API key, city and unit in the WEB interface.

The weather condition codes (more detailed than the weather icons) are determined from the JSON response and the corresponding icons (which are designed by myself especially for a matrix display) are displayed. Most icons exist for day and night. A binary search is applied to the icon array when selecting one icon. 

### Motion detection

The status of the PIR sensor is read cyclic. Immediately when a motion is detected the display shows all graphics. The hold time (when no motion is detected anymore) until the display is drawn completely black can be configured in the WEB interface (default: 10 min). Because the range is very poor an alternative would be to use a short range radar for motion detection. Until this is not completely clarified the timeout of the hold time is disabled in the source code to leave the display permanently on.

### HTTP server

The WEB interface is used to store the settings of OpenWeatherMap API, NTP server, time zone and motion detection hold time in the EEPROM. The assigned IP can be found in the router settings. The WIFI settings can be resetted with http:/<IP>/resetWifi. An access point is started with the next reset (SSID: matrix_weather_clock IP: 192.168.1.1) and new WIFI settings can be configured.

### Drawing

The display refresh cycle for the [PxMatrix](https://github.com/2dom/PxMatrix) library is configured to 2 ms. The drawing cycle is configured to 100 ms. Within these 100 ms all graphical elements are drawn. A temporary buffer of 1kbyte (32 columns x 16 rows x 2 bytes color) is used as a double buffer, to which every graphical element can draw in its draw cycle. At the end of the draw cycle the complete buffer is copied to the PxMatrix interface to control the LEDs. At the beginning of each draw cycle the buffer is completely cleared. All graphical text elements can be drawn with an individual color. The 100 ms cycle time has been choosen because of the animation effects like colon blinking, digit- and menue-scrolling. Menue scrolling is triggered by a push button event.

## Photos

Actual weather icon and weather forecast icons (default view)
(the PIR sensor is visible in the upper left corner, the push button at the upper side of the wooden frame is not visible)

![Weather](/photos/weather.jpg)

Humidity view (visible after the 1st press of the push button)

![Humidity](/photos/humidity.jpg)

Air pressure view (visible after the 2nd press of the push button)

![Pressure](/photos/pressure.jpg)
