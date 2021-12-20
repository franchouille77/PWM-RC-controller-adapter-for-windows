May 2020

# PWM RC receiver adapter for Windows using Arduino Leonardo (like USB XBOX controller using XINPUTS)
## INTRODUCTION

This code allow to use a PWN RC receiver on a computer running on windows 10 (and probably other version of windows) with model simulators (tested with with Flying Model Simulator and with FPV drone simulator), it works with an arduino leonardo and the ArduinoXInputs library of Dave MADISON, that's why windows recognise the arduino (and by consequent the RC receiver) as an XBOX controller which is highely compatible with windows (I can use my turnigy 9X radio controller to navigate directly into the OS).

It is very handy to use as it don't need any modification on your radio controller and it is wireless. At the end you will get a very simple adapter, with the aim to just work fine on windows, the only negative point is the first (but last) configuration for the arduino to be deteced as an XBOX controller.

My intent was to use my radio contoller with 4 channels for the 2 joysticks, and map them to be x and y axis of both left and right joystiks of an XBOX controller, and that's all. So if you want to use more channels from the receiver you need to wire more channels to the arduino and map them with the corespondant output fonctions of the ArduinoXInputs library.

I will explain bellow how to quick setup for arduino leonardo, how to use it, how to adapt it for other AduinoXInputs compatible boards and how to use more channels from your RC receiver (for exemple my Turnigy 9X8Cv2 receiver has 8 PWM channels that can be used with the 2 joysticks, knobs, and switches of my turnigy 9X radio controller).

## QUICK SETUP ON ARDUINO LEONARDO

The following steps of this section went from the Dave MADISON's Website, I highly recommand you to read his tutorial because it's the original tutorial and it comport a lot of details about other compatible boards and all the knowledge about how to setup and use his library, the link is: https://www.partsnotincluded.com/how-to-emulate-an-xbox-controller-with-arduino-xinput/

Now that you know this, if you only want informations about the quick upload to an arduino leonardo board you can read what's next:

Thanks to the informations provided by the Dave MADISON's tutorial on how to use his ArduinoXInputs library, the stages to make it run on an Arduino Leonardo are as follows:

### 1) INSTALL BOARD PACKAGE FOR ARDUINO LEONARDO

Download board package at this link: https://github.com/dmadison/ArduinoXInput_AVR

To install the package, navigate to the directory containing your Arduino installation, and then open up the 'hardware' folder. Extract the contents of the .zip file that you donwloaded into this directory. You should have a new 'xinput' folder with an 'avr' folder inside of it, containing the files from this repository.

Restart the Arduino IDE. If the XInput AVR core is installed correctly, you should see a new collection of "XInput AVR Boards" in the 'Tools -> Boards' menu.

### 2) INSTALL XINPUT LIBRARY

Download the ArduinoXInput library at this link: https://github.com/dmadison/ArduinoXInput
You can install this like any other Arduino library, download the latest release as a .zip file and use the IDE’s library manager to install it ( Sketch -> Include Library -> Add .ZIP Library ). Or you can also unzip it into 'Documents\Arduino\libraries'.

 ### 3) TRY TO UPLOAD AN EXAMPLE XINPUT SKETCH

We need to install an example skecth to be sure that the board is detect as an XBOX controller with windows

In arduino IDE go to 'File -> Prefencences' and turn on 'verbose output' for uploading.
Then go to 'Tools -> Board' and select the 'Arduino Leonardo w\ XInput'and select the port of your Arduino Leonoardo board.
Then go to 'File -> Examples -> XInput' and select the 'Blink' example sketch.

Upload the sketch with the following steps:
* Press the 'Upload' button in the IDE
* Wait until the status bar says "Uploading…"
* Press the reset button of your Arduino Leonoardo board twice, quickly
If you did these steps properly, the board should reset to the bootloader and the upload should begin. AVRDUDE will do it's thing and you should see "avrdude done. Thank you." near the bottom of the output window. Note that the IDE may say that it "Couldn’t find a Board on the selected port" even if the upload succeeded. (Does not matter if not working well, now go on next step, but if next step fails, try upload another time).

After that been done, go to 'start -> settings -> devices -> devices and printers', if you did it right you should see here an xbox controller in the list of available devices, then right click on this device, then 'settings -> properties' and now you should be able to see Button 1 blink once a second (if not try rebooting your computer and upload the sketch again).

### 4) UPLOAD THE 'RC RECEIVER ADAPTER FOR WINDOWS' SKETCH

Now that you are sure the arduino Leonardo works as a XBOX controller, it' time to upload the 'RCreceiverAdapterForWindows.ino' into the Arduino Leonoard for that the steps are the follows:

Close Arduino IDE, then unplug the Arduino Leonardo from computer, then plug it and start Arduino IDE.
Now download from this repository and load into the Arduino IDE the 'RCreceiverAdapterForWindows.ino' sketch.

Then upload the sketch with the following steps:
* Press the 'Upload' button in the IDE
* Wait until the status bar says "Uploading…"
* Press the reset button of your Arduino Leonoardo board twice, quickly

If you did these steps properly, the board should reset to the bootloader and the upload should begin. AVRDUDE will do it's thing and you should see "avrdude done. Thank you." near the bottom of the output window. Note that the IDE may say that it "Couldn’t find a Board on the selected port" even if the upload succeeded.

After that been done, if you did it right, you should be able to see the 'L' labelled led on the boad blink once a second, and if you go to 'start -> settings -> devices -> devices and printers', you should be able to see here an xbox controller in the list of available devices (if not try rebooting your computer and upload the sketch again).

## USER MANUAL

### 1) WIRING

You need to wire channels 1, 2, 3, 4 of the RC receiver to PINs 11, 10, 9, 8 of the arduino in this order, and use 5v and GND from arduino to power the RC receiver, and as you need to go on calibration mode for the first start, you need to connect A0 pin to ground, as you can see below:


                                               +-----+
                  +----[PWR]-------------------| USB |--+
                  |                            +-----+  |
                  |                                     |
                  |                           A5/SCL[ ] |  .
                  |                           A4/SDA[ ] |  .
                  |                             AREF[ ] |  .
                  |                              GND[ ] |  .
                . | [ ]N/C                    SCK/13[ ] |  .            __________________________________________
                . | [ ]IOREF                 MISO/12[ ] |  .           |+ - S                                    |
                . | [ ]RST                   MOSI/11[ ]~|  .----------------.  CH1                               |
                . | [ ]3V3                        10[ ]~|  .----------------.  CH2                               |
          +-----. | [ ]5v                          9[ ]~|  .----------------.  CH3                               |
          | +---. | [ ]GND     +----------+        8[ ] |  .----------------.  CH4                               |
          | | +-. | [ ]GND     | ARDUINO  |             |              |. . .  CH5         PWM RC RECEIVER       |
          | | | . | [ ]Vin     | LEONARDO |        7[ ] |  .           |. . .  CH6                               |
          | | |   |            +----------+        6[ ]~|  .           |. . .  CH7                               |
          | | +-. | [ ]A0                          5[ ]~|  .           |. . .  CH8                               |
          | |   . | [ ]A1                          4[ ] |  .           ||_|______________________________________|
          | |   . | [ ]A2                     INT1/3[ ]~|  .            | |
          | |   . | [ ]A3                     INT0/2[ ] |  .            | |
          | |   . | [ ]A4/SDA  RST SCK MISO     TX>1[ ] |  .            | |
          | |   . | [ ]A5/SCL  [ ] [ ] [ ]      RX<0[ ] |  .            | |
          | |     |            [ ] [ ] [ ]              |               | |
          | |     | LEONARDO   GND MOSI 5V  ____________/               | |
          | |      \_______________________/                            | |
          | |                                                           | |
          +-|-----------------------------------------------------------+ |
            +-------------------------------------------------------------+



### 2) FIRST START IN CALIBRATION MODE

Calibration mode is the first thing you have to do, as the program don't know the motion range of the joysticks of your radio-emitter, that is represented by the delta of minimums and maximums values it receives from each channel of the radio-receiver connected to it.

To do this be sure you directly connect pin A0 to ground (no need to use a pullup resistor), power-on your radio-emitter and then you can connect the arduino leonardo to the computer. when entering into calibration mode the 'L' labelled LED on the board should turn solid ON.
When you are into calibration mode you have to move both joysticks (right and left) on your radio emitter in the full range of motion for both axis (X and Y), and be sure you do it many times, as the arduino will record the minimum and maximum values from both axis for both joysticks. When you are done simply disconnect pin A0 from ground, and then 'L' labelled led on the board should turn OFF and 'TX' labelled LED should turn ON.
From now you should be able to see an XBOX controller into the the devices list from 'start -> settings -> devices -> devices and printers' and if you right click on this device, then 'settings -> properties' and now you should be able to see the point in the square moving when you move the left jotstick, and you should be able to see the values in the cursors moving when you move the right joystick.

### 3) TROUBLESHOOTING
If the 'L' labelled led is blinking once a second, whether in calibration mode or in regular mode, that's because the Arduino Leonardo receive a '0' value from at least one of the 4 channels, so double check your wiring and be sure that your radio-emitter is tuned ON (when my turnigy 9x radio-emitter is turned on I can see a red led turned on inside of my radio-receiver).


### 4) REGULAR USE

From now you should be able to use your radio-receiver as an XBOX controller in your favorite radio controlled model simulator. for the next times that you will use this adapter, you will not need to enter into calibration mode anymore if you use the same radio-receiver with the same radio-emitter as the values from the previous calibration was stored into the EEPROM memory of the arduino, and even more it is recommended to use the calibration mode as infrequently as possible, as the EEPROM memory only have 100000 cycles of erasing/writting. So you can definitively disconnect the A0 pin from ground and the wiring becomes as below:


                                               +-----+
                  +----[PWR]-------------------| USB |--+
                  |                            +-----+  |
                  |                                     |
                  |                           A5/SCL[ ] |  .
                  |                           A4/SDA[ ] |  .
                  |                             AREF[ ] |  .
                  |                              GND[ ] |  .
                . | [ ]N/C                    SCK/13[ ] |  .            __________________________________________
                . | [ ]IOREF                 MISO/12[ ] |  .           |+ - S                                    |
                . | [ ]RST                   MOSI/11[ ]~|  .----------------.  CH1                               |
                . | [ ]3V3                        10[ ]~|  .----------------.  CH2                               |
          +-----. | [ ]5v                          9[ ]~|  .----------------.  CH3                               |
          | +---. | [ ]GND     +----------+        8[ ] |  .----------------.  CH4                               |
          | |   . | [ ]GND     | ARDUINO  |             |              |. . .  CH5         PWM RC RECEIVER       |
          | |   . | [ ]Vin     | LEONARDO |        7[ ] |  .           |. . .  CH6                               |
          | |     |            +----------+        6[ ]~|  .           |. . .  CH7                               |
          | |   . | [ ]A0                          5[ ]~|  .           |. . .  CH8                               |
          | |   . | [ ]A1                          4[ ] |  .           ||_|______________________________________|
          | |   . | [ ]A2                     INT1/3[ ]~|  .            | |
          | |   . | [ ]A3                     INT0/2[ ] |  .            | |
          | |   . | [ ]A4/SDA  RST SCK MISO     TX>1[ ] |  .            | |
          | |   . | [ ]A5/SCL  [ ] [ ] [ ]      RX<0[ ] |  .            | |
          | |     |            [ ] [ ] [ ]              |               | |
          | |     | LEONARDO   GND MOSI 5V  ____________/               | |
          | |      \_______________________/                            | |
          | |                                                           | |
          +-|-----------------------------------------------------------+ |
            +-------------------------------------------------------------+


Next section will come later...
