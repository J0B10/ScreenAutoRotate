# ScreenAutoRotate

A Tray application written in c++ and java that rotates the screen whenever the monitor is rotated.

An Arduino nano with an [MPU 6050 sensor](https://playground.arduino.cc/Main/MPU-6050/) is glued with double sided tape to the back of the monitor to detect any motions.
If it detects that the monitor is rotated it sends an instruction to the PC via USB which then triggers the application to rotate the display.

All used arduino scripts can be found in the `ArduinoScripts` directory.

### The hardware
<img src="arduino.jpg" width="300" height="400"/>
