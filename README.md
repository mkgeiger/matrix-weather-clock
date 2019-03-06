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
* all individual settings like timezone (incl. daylight saving and nornal time), OpenWeatherMap API key, OpenWeatherMap API city, OpenWeatherMap API units, NTP server address and motion detection hold time shall be configurable with a WEB server providing a settings page.
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

This is a LED matrix display which has in total 512 (32x16) RGB LEDs. If you want to learn more about the internal electronic details of such a display, please visit this page: https://bikerglen.com/projects/lighting/led-panel-1up/#The_LED_Panel. The setup and cabling for usage with the PxMatrix driver you should visit the page: https://github.com/2dom/PxMatrix/blob/master/README.md and https://learn.adafruit.com/32x16-32x32-rgb-led-matrix/new-wiring. For powering please visit this page: https://learn.adafruit.com/32x16-32x32-rgb-led-matrix/powering. 

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

## Software

## Photos

Actual weather icon and weather forecast icons (default view)
(the PIR sensor is visible in the upper left corner, the push button at the upper side of the wooden frame is not visible)

![Weather](/photos/weather.jpg)

Humidity view (visible after the 1st press of the push button)

![Humidity](/photos/humidity.jpg)

Air pressure view (visible after the 2nd press of the push button)

![Pressure](/photos/pressure.jpg)
