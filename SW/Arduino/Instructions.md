In order to program the Arduino Board you have to follow these instructions.

1) Install the Arduino Desktop IDE

Instructions are available at https://www.arduino.cc/en/Guide/HomePage

2) Installing Additional Arduino Cores

Only AVR Arduino boards are installed by default. 
Some Arduino boards require an additional Core to be installed (e.g. the Arduino Due used in this work)
In order for the Arduino IDE to be able to program an Arduino Due, you need to install the SAM Core 
using the Boards Manager. Instructions are reported here: https://www.arduino.cc/en/Guide/Cores

3) Copy the sketch and the library

Copy the Arduino sketch ADS1298_ArduinoDue in the folder
/Users/<username>/Documents/Arduino/ADS1298_ArduinoDue/
Be aware that the name of the sketch and the name of the last folder must be the same. So if you decide
to rename the sketch you have to do the same with the last folder

Copy the library folder in:
/Users/<username>/Documents/Arduino/libraries/

4) Upload the software on Arduino

Connect your Arduino DUE programming port to your PC or MAC via USB
Before uploading your sketch, you need to select the correct items from the Tools > Board and Tools > Port menus. 
Once you've selected the correct serial port and board, press the upload button in the toolbar or select the Upload item from the Sketch menu. 
Current Arduino boards will reset automatically and begin the upload. You'll see the RX and TX LEDs blink as the sketch is uploaded. 
The Arduino Software (IDE) will display a message when the upload is complete.
For further information follows the instructions reported here: https://www.arduino.cc/en/Guide/Environment#toc9

5) Usege of Creamino
Attach the shield to the Arduino board. Use the Native port to acquire EEG signals and the Programming port to send the trigger.

For further information refer to the paper which describes several setup experiments.
