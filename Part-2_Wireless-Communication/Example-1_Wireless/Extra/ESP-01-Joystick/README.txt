To install the ESP8266 features:

Start Arduino and open the Preferences window.
Enter https://arduino.esp8266.com/stable/package_esp8266com_index.json into the File>Preferences>Additional Boards Manager URLs field of the Arduino IDE. You can add multiple URLs, separating them with commas.
Open Boards Manager from Tools > Board menu and install esp8266 platform (and don't forget to select your ESP8266 board from Tools > Board menu after installation).

In order to use the libraries and tools for this sketch, pull the folders from "libraries-and-tools" into the Arduino sketchbook folder, e.g.:

C:\user\Documents\Arduino\

or

/home/user/Arduino/

etc.

The Sketch should be uploaded to the board type: "Generic ESP8266 Module"
Then when the sketch has been uploaded, upload ther SPIFFS content from the contained "data" folder. This contains the webpage to be served.

The option is located:
Tools > ESP8266 Sketch Data Upload

It may respond with:
esptool.FatalError: Timed out waiting for packet header
SPIFFS Upload failed!
It still went through, don't worry

Then the sketch should work. You can always check for working sketch by going to the Access Point and IP address shown in the Serial Monitor.