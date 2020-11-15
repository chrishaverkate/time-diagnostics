# Arduino TimeDiagnostics library

# Overview
Provides a diagnostics framework for timing on the Arduino. 

## Simple Usage
Create one TimeDiagnostics object for the lifetime of your related measurements and start new timers to measure various sections.

Example:
```c++
#include <time_diagnostics.h>

TimeDiagnostics td;

{ // new scope limiter
    START_TIMER(td, timer_id);
    // Your code section
    //...
 } // timer will finish and post because the object is destroyed at the end of the scope.

// You can print the time recorded (in microseconds).
Serial.print(td.timer_measurement(timer_id));
```

# Examples
## Basic
examples/[basic_time_diagnostics.ino](https://github.com/chrishaverkate/time-diagnostics/blob/develop/examples/basic_time_diagnostics/basic_time_diagnostics.ino) is a working Arduino sketch and a good starting point. It will demonstrate:
* Setting up a TimingDiagnostics object
* Defining timer IDs
* Using two timers
* Basic printing of the results  


## Advanced
examples/[advanced_time_diagnostics.ino](https://github.com/chrishaverkate/time-diagnostics/blob/develop/examples/advanced_time_diagnostics/advanced_time_diagnostics.ino)) is a working Arduino that expands on the basic example to show:
* Associating timer IDs with printable names
* Simple CSV reporting
* Disabling Timing Diagnostics for releasable code

# Contributing
## Setting up Build
### Arduino SDK Path
In order build, I found I needed to have `ARDUINO_SDK_PATH` set in the environment and for CMake.
 
For CMake: `-DARDUINO_SDK_PATH=/home/user/arduino-1.8.13/`<br>
For environment: `export ARDUINO_SDK_PATH=/home/user/arduino-1.8.13/`

If you do not have this set you'll get the following error: ```Could not find Arduino SDK (set ARDUINO_SDK_PATH)!```


