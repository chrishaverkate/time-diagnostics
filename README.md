# Arduino TimeDiagnostics library

#Getting Started
###Arduino SDK Path
In order build, I found I needed to have `ARDUINO_SDK_PATH` set in the environment and for CMake.
 
For CMake: `-DARDUINO_SDK_PATH=/home/chris/devbin/bin/arduino-1.8.13/`<br>
For environment: `export ARDUINO_SDK_PATH=/home/chris/devbin/bin/arduino-1.8.13/`

If you do not have this set you'll get the following error: ```Could not find Arduino SDK (set ARDUINO_SDK_PATH)!```


