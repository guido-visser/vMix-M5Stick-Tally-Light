# vMix M5Stick-C Tally Light

![](working-example.gif)

## Features
* ⚡  Press on M5 button will toggle between 3 screens:
	1. The actual status of the tally
	2. Network screen, showing the current IP Address so you can access the Web UI
	3. Show the actual tally input number that is set for the unit
* ⚡  Webserver for easy configuration
* ⚡  Access Point mode when not connected to a WiFi network
* ⚡  Data is saved to memory, will be remembered after device is turned off
* ⚡  Dedicated button to reconnect to vMix (BTN39)
* ⚡  Holding the same button at the end of trying to reconnect will reset the settings
* ⚡  Dark Web UI

---
## Getting started: Uploading the code to the stick

### Video tutorial
[![Video Tutoral](http://img.youtube.com/vi/uB_OkMGLoto/0.jpg)](http://www.youtube.com/watch?v=uB_OkMGLoto "How to update the firmware of the M5Stick C to the Tally Light project")

### Step 1
1.1  Follow the tutorial on the [M5Stack website](https://docs.m5stack.com/#/en/arduino/arduino_development).

1.2  Copy the libraries that are used by this project (found in the `libs` folder) to your system. The default path is: `%userprofile%\Documents\Arduino\libraries`

### Step 2
Upload to your device.

### Step 3
Follow the instructions on the LCD.

### Step 4
Enjoy 😄

---
## Configuring the stick
1. Once the code is uploaded to the stick, wait until it's in AP (Access Point) mode.
2. Grab a phone and connect to the `vMix-M5Stick-Tally` wifi network, the password is: `12345678`.
3. Open your browser and go to: `http://192.168.4.1`, this will open up the Web UI.
	1. Enter the SSID of the wifi network you want to connect to.
	2. Enter the password that corresponds to the SSID.
	3. Enter the IP Address of the PC that's running vMix.
	4. Specify the input number that the stick must listen to. (This is the little number in the top left corner of every input)
4. Hit the save button.
5. The stick will restart with the given information. If you changed the wifi settings, it's recommended to manually reboot the stick (turn off and on again).

---
## TODO
* [ ]  Configuration tutorial
	* [x] Written
	* [ ] Video
* [ ]  Option to set the brightness of the LCD
* [ ]  Use accelerometer to use it in landscape or portrait
* [ ]  Multiple inputs for a single stick
* [ ]  Add battery indicator (need help)

### Good to know
This is my very first C-project, if you have any suggestions on how to improve the code, please let me know.

#### Thanks
A massive thanks to [Thomas Mout](https://github.com/ThomasMout) for creating [Arduino vMix Tally](https://github.com/ThomasMout/Arduino-vMix-tally). This project is based on his.

#### Support the project
This project is completely free to use in any environment (commerically or non-commerically). However, if you like it and use it regularly, please consider buying me a coffee 😄

<a href="https://www.buymeacoffee.com/guidovisser" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/default-orange.png" width="217" height="51" alt="Buy Me A Coffee" style="border-radius: 5px" ></a>
