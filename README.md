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

## Hardware

## Software

## Photos

Actual weather icon and weather forecast icons (default view)
(the PIR sensor is visible in the upper left corner, the push button at the upper side of the wooden frame is not visible)

![Weather](/photos/weather.jpg)

Humidity view (visible after the 1st press of the push button)

![Humidity](/photos/humidity.jpg)

Air pressure view (visible after the 2nd press of the push button)

![Pressure](/photos/pressure.jpg)
