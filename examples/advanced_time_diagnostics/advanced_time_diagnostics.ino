
// To disable the diagnostics, just define the macro below. No functions will be called. This is a good idea for release ready code.
// Note: This must be defined before the include of time_diagnostics.
// #define DISABLE_TIME_DIAGNOSTICS

#include <time_diagnostics.h> ///< The only included needed.

// Create a TimeDiagnostics object that is capable of holding 4 time recordings.
// If more timers are needed, just increase the number. There is no internal limit, it just depends on how much
// space you want to set aside for recording time.
TimeDiagnostics<4> time_diag;

// An example of how to keep track of timer IDs.
// Note: the order of these IDs need to match for their names below.
enum TimerIdentity {
  function_loop = 0
  , function_count_100
  , function_count_random
  , Invalid
};

// An example of attaching a printable name to each timer.
// Note: keep them in order with the identities above.
static const char* TIMER_NAMES[] = {
  "Function: Loop"
  , "Function: Counting to 100"
  , "Function: Counting to random number"
  , "Invalid"
};

void setup() {
  // Turn on serial to print out timing data.
  Serial.begin(115200);

}

// Example of how to report the recorded timers in CSV format.
void report() {
#ifndef DISABLE_TIME_DIAGNOSTICS
  Serial.println("Timer ID,Microseconds");

  // Get total number of timer reports
  const unsigned int TIMER_COUNT = time_diag.number_of_reports();

  // Loop over all timers, getting their recorded ID and measurement
  for(int timer_index(0); timer_index < TIMER_COUNT; ++timer_index) {
    int id = -1;
    unsigned long micros = 0;

    if(time_diag.timer_report(timer_index, id, micros)) {
      Serial.print(TIMER_NAMES[id]);
      Serial.print(",");
      Serial.println(micros);
    }
  }
  Serial.println("");
#endif
}

void loop() {
  Serial.println("====================start=============================");
  unsigned long start = micros();

  // Create a new scope for the work that loop() will do.
  {
    START_TIMER(time_diag, function_loop);
    Serial.println("Hello world!");

    count_to_100();
    count_to_random();

    delay(2000);
  }

  // Report needs to happen outside the scope so that the function_loop timer
  // will have a chance to report it's measurement.
  report();

  // Reset the timer logging before the next loop.
  // If this doesn't happen, old timer information will persist and eventually, no new times will be recorded.
  time_diag.reset();

  unsigned long end = micros();
  Serial.print("Raw time for loop(): ");
  Serial.println(end - start);
  Serial.println("=====================end==============================");
}

void count_to_100() {
  START_TIMER(time_diag, function_count_100)

  volatile long x; // just some math to take time
  for(int i(0); i < 100; ++i) {
    x = i / 2;
  }
}

void count_to_random() {
  START_TIMER(time_diag, function_count_random)

  randomSeed(analogRead(0));
  long n = random(1, 10) * 10000;

  volatile long x;
  for(long i(0); i < n ; ++i) {
    x = i / 2;  // just some math to take time
  }
}
