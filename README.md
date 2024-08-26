# ESP32Joystick
A wifi joystick with the interface hosted on an esp8266
(Despite the name "ESP32Joystick" it uses an ESP8266, it is because I intended an ESP32 to be used at first, but it got too complicated)

This esp8266 joystick has an interface made with html, css and javascript which are hosted on a WiFi capable microcontroller, the esp8266. The esp8266 communicates with an Arduino Pro Micro which emulates the Xbox 360 HID controller over I2C bus protocol.

![ESP32Joystick](https://github.com/YeetTheAnson/ESP32Joystick/raw/main/1.png)
![ESP32Joystick](https://github.com/YeetTheAnson/ESP32Joystick/raw/main/2.jpg)

https://github.com/user-attachments/assets/3ea3ca40-7c02-4e33-9a6d-51664f51ecf4

# Getting started

1. Clone the repository with ```git clone https://github.com/YeetTheAnson/ESP32Joystick```
2. Navigate to the main folder of the repository and flash
    - `ESPJOYSTICK.ino` to your [ESP8266](#flashing-esp8266)
    - `ArduinoCode.ino` to your [Arduino Pro Micro](#flashing-arduino-pro-micro)
3. To use this, follow the [circuit diagram](#circuit-diagram)
4. Learn how to use it [here](#usage)




## Flashing ESP8266

1. Navigate to `ESPJOYSTICK` and open the `ESPJOYSTICK.ino` file in Arduino IDE
2. Set the configuration by changing the SSID and password in the code
3. Add `ESP8266` to the board manager:
    - Go to `File` >> `Preferences` >> `Additional Boards Manager URLs`
    - Paste the following URL: `https://arduino.esp8266.com/stable/package_esp8266com_index.json`
4. Open board manager download `esp8266` by esp8266 community
5. Select the board `NodeMCU 1.0 (ESP-12E Module)`
6. Compile and flash the code to the ESP8266


## Flashing Arduino Pro Micro

1. Navigate to `ArduinoCode` and open the `ArduinoCode.ino` file in Arduino IDE
2. Add `Arduino Leonardo w/ Xinput` to the board manager:
    - Go to `File` >> `Preferences` >> `Additional Boards Manager URLs`
    - Paste the following URL: `https://raw.githubusercontent.com/dmadison/ArduinoXInput_Boards/master/package_dmadison_xinput_index.json`
4. Open board manager download `Xinput AVR Boards` by David Madison
5. Select the board `Arduino Leonardo w/ Xinput`
6. Open the library manager and download Xinput
7. Compile and upload the code
8. During the 'Uploading...' stage, press the reset button twice quickly


## Circuit Diagram

| Arduino Pro Micro (5v) | ESP8266         |
|------------------------|-----------------|
| Pin 3                  | D1 / GPIO5       |
| Pin 2                  | D2 / GPIO4       |
| GND                    | GND              |
| VCC                    | VU               |


# Usage

1. Connect to the WiFi SSID and Password you set in the code (Default SSID and Password is `WirelessJoystick`)
2. Open your browser and type in `192.168.4.1` in your address bar

## Features
- Has all buttons on the XBOX 360 controller
