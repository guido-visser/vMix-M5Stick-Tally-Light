# vMix M5Stick-C Tally Light

![](working-example.gif)

## Features

-   🚀 Press on M5 button will toggle between 4 screens:
    1.  The actual status of the tally (a tally status change will default back to this screen)
    2.  Network screen, showing the current IP Address so you can access the Web UI (+ a wifi strength indicator)
    3.  Show the actual tally input number that is set for the unit
    4.  The brightness screen. You can use BTN39 on this screen to toggle through the 9 different settings
-   🚀 Webserver for easy configuration
-   🚀 Access Point mode when not connected to a WiFi network
-   🚀 Data is saved to memory, will be remembered after device is turned off
-   🚀 Dedicated button to reconnect to vMix (BTN39, will reconnect to vMix except on the brightness screen)
-   🚀 Holding the same button at the end of trying to reconnect will reset the settings
-   🚀 Dark Web UI
-   🚀 Ability to adjust the brightness (either on the stick itself, or via the Web UI)
-   🚀 Battery indicator
-   🚀 Landscape & Portrait mode using accelerometer
-   🚀 Change tally number input on the fly. On the tally input number screen, double click the BTN39 to increase the number. Long click for reset to 1
-   🚀 Multi input support
-	🚀 Automatic wifi scanning to easily select your wifi network
-	🚀 Set an interval to automatically reconnect to vMix after the interval has passed
-	🚀 Option to just show the LIVE state and ignore the PRE and SAFE states
-	🚀 Option to show the tally number instead of the SAFE, PRE and LIVE texts
-   🚀 Indicators for Streaming (STM) and Recording (REC)

---

## FAQ

**Q: Does this work with ATEM, TriCaster, OBS Studio and others?**
A: Yes, however [Tally Arbiter](https://github.com/josephdadams/TallyArbiter) is required for this functionality. The plan is to integrate seemless with TA. Stay tuned for updates.

## Getting started: Uploading the code to the stick

### Video tutorial

[![Video Tutoral](http://img.youtube.com/vi/uB_OkMGLoto/0.jpg)](http://www.youtube.com/watch?v=uB_OkMGLoto "How to update the firmware of the M5Stick C to the Tally Light project")

### Step 1

1.1 Follow the tutorial on the [M5Stack website](https://docs.m5stack.com/#/en/arduino/arduino_development).

1.2 Copy the libraries that are used by this project (found in the `libs` folder) to your system. The default path is: `%userprofile%\Documents\Arduino\libraries`

### Step 2

Upload to your device.

##### When using the original M5Stick-C

You don't have to change the code! 🎉

##### When using M5Stick-C PLUS

Change the very first line in `a_GLOBAL.ino` to `#define C_PLUS 1` (change the `0` to a `1`)
That's it!

### Step 3

Follow the instructions on the LCD.

### Step 4

Enjoy 😄

---

## Updating the firmware

### IMPORTANT BEFORE UPDATING TO V2.5.0

The firmware update logic allows any arduino compiled binary to be uploaded and flased to the stick. I decided to password protect this feature, since it's unwanted that a random person starts to upload random firmware to the stick.

**CHANGE THE DEFAULT PASSWORD**
You need to change the `UPDATE_PASSWORD` manually! 

### Update to v2.5.0 via the old way

This version has the firmware upload logic, meaning from this version onwards, you can update the firmware wirelessly instead of needing to connect it to a computer. 

### How to update the firmware after v2.5.0?

This documentation will be updated soon!

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
5. The stick will restart with the given information.

### Fields explained
**SSID**

A select dropdown which shows a list all the wifi networks found.

**Hidden SSID Name**

This field only shows when you select `Hidden network` in the SSID list. 
Type in the SSID that you want to connect to.

**Password**

The password that corresponds to the selected wifi network.

**vMix IP Address**

Fill in the IP Address of the computer where vMix is running. 

How to find the IP of the vMix PC:
1. Press Win + R
2. Type in `cmd` and hit enter. A commandprompt will appear
3. In the commandprompt type `ipconfig` and press enter
4. Your IP Address can be found behind the `IPv4 Address. . . . . . . . . . . : ` line

**Main Tally Number**

This field corresponds to the input number in vMix. Every input has a number in the top left corner of the video tile. Enter the input number you want the tally to respond to.

**Multi Input**

It's possible to have more inputs hooked into 1 tally light, you can provide a comma separated string to set this up.

Example: 

When the Main Tally Number is set to `1` and Multi Input is set to `2,5`, this will be the behavior:

SAFE:

None of the inputs are in Preview or in the Program feed.

PRE:

One of the inputs is in Preview, none are on Program.

LIVE:

One of the inputs is on the Program feed. Live always has the highest priority.

**Reconnect interval**

Input is in seconds. This will try to reconnect to vMix every `x` seconds after the interval has passed when it's disconnected. This will loop if the connection cannot be established. 

WARNING:
Since the stick is single threaded, when the stick tries to reconnect to vMix, the Web UI is not accessible! 

Example:
When set to `10` it will try to reconnect to vMix every 10 seconds after connection was unsuccessful.

**Brightness**

Set the brightness of the LCD. Option to choose from:
* 0%
* 20%
* 40%
* 60%
* 80%
* 100%

NOTE: Obviously, the battery will drain faster when the brightness is set to a high value. I recommend setting it at 60%.

**Just Live**

`true`: The stick only lights up when the configured input(s) is/are live.

`false`: The stick shows when the configured input(s) is/are safe, are in preview or when it is live.

**Mode**

`Text (SAFE, PRE, LIVE)`: Show the textual representation of the status of the configured input(s).

`Tally Number`: Show the configured main tally number and only update the background color when the status changes.

---

## Static IP Address

**WARNING**: This is only for users that know what they're doing. If you are not familiar with ip addresses, gateways or subnets, stay away.

It is possible to have a static IP Address, however it's not possible (yet) to do this via the Web UI. You need to hard-code this. A template is provided, but you'll have to change some code:

-   Open `e_WIFI.ino`
-   Remove the lines that have `REMOVE THIS LINE WHEN YOU NEED STATIC IP` in them (be sure to also remove the comment markers `/*` and `*/`)
-   Change the settings after the `IPAddress` lines that meet your needs in your environment.

## Plugins

Officially supported plugin:

-   LED HAT (for the HEX and NEOFLASH HATs) Big thanks to [Dirwin Clemens](https://github.com/Dirwinc)

For more information see [Plugins.md](https://github.com/guido-visser/vMix-M5Stick-Tally-Light/tree/master/src/a_GLOBAL/plugins/Plugins.md)

---

### Forks

Since M5Stack has a lot of different products, some wonderful people forked my code to use it on those different products. Below is a list of products supported by the community:

##### [M5Stack ATOM](https://github.com/MarkGaretz/ATOM-Matrix-Tally-Light) by Mark Garetz

##### [M5Stack CORE](https://gitlab.com/ptellep/m5stack-tally-for-vmix) by Peter Tellep

---

## TODO

-   [ ] Create a seemless integration with [Tally Arbiter](https://github.com/josephdadams/TallyArbiter)
-   [ ] Configuration tutorial
    -   [x] Written
    -   [ ] Video

### Good to know

This is my very first C-project, if you have any suggestions on how to improve the code, please let me know.

#### Thanks

A massive thanks to [Thomas Mout](https://github.com/ThomasMout) for creating [Arduino vMix Tally](https://github.com/ThomasMout/Arduino-vMix-tally). This project is based on his.

#### Support the project

This project is completely free to use in any environment (commerically or non-commerically). However, if you like it and use it regularly, please consider buying me a coffee 😄

<a href="https://www.buymeacoffee.com/guidovisser" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/default-orange.png" width="217" height="51" alt="Buy Me A Coffee" style="border-radius: 5px" ></a>
