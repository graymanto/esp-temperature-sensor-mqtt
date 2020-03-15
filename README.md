# ESP8266 Temperature Sensor

This repository contains the code for an ESP8266 based temperature sensor.


## Required hardware

The code assumes you are using nodemcu or esp01 board with a DS18B20 temperature sensor attached on pin D4 for the nodemcu or GPIO 2 for the esp01. 

It also assumes the board is setup to allow deep sleep.
 
## What does it do

The board should wake from deep sleep every 2 minutes (or whatever length is configured), read the temperature and current time then make To an MQTT server.


## Installation and upload
### Install required dependencies

```bash
pio lib install DallasTemperature
```

### Setting runtime values

Before building, the PLATFORMIO\_BUILD\_FLAGS environment variables needs to be set correctly to set the SSID, wifi password, sleep time etc. For an example of how to do this, see the file setbuildvars.example.sh.

The easiest approach is to copy this file to setbuildvars.sh, set the values in the file as appropriate then run

```bash
source setbuildvars.sh
```

You can then proceed with the build.

### Build and upload

To make and upload for the nodemcuv2

```bash
make build-nmcu
make upload-nmcu
```

For the esp01

```bash
make build-nmcu
make upload-nmcu
```
