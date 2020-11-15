
#include <time_diagnostics.h> ///< The only included needed.

// Create a TimeDiagnostics object that is capable of holding 4 time recordings.
// If more timers are needed, just increase the number. There is no internal limit, it just depends on how much
// space you want to set aside for recording time.
TimeDiagnostics<4> time_diag;
static const int ID_FUNCTION_LOOP = 0;
static const int ID_FUNCTION_COUNT = 1;

void setup() {
  // Turn on serial to print out timing data.
  Serial.begin(115200);
}

void loop() {
  Serial.println("====================start=============================");
   
  // Create a new scope for the work that loop() will do.
  {
    START_TIMER(time_diag, ID_FUNCTION_LOOP);
    Serial.println("Hello world!");

    count_to_random();
    
    delay(2000);
  }

  // Print the times that were recorded for the two timers.
  Serial.println("Time Report in microseconds:");
  Serial.print("Random counting: ");
  Serial.print(time_diag.timer_measurement(ID_FUNCTION_COUNT));
  Serial.print("\nMain Loop:       ");
  Serial.println(time_diag.timer_measurement(ID_FUNCTION_LOOP));

  // Reset the timer logging before the next loop. 
  // If this doesn't happen, old timer information will persist and eventually, no new times will be recorded.
  time_diag.reset();

  Serial.println("=====================end==============================");
}

void count_to_random() {
  START_TIMER(time_diag, ID_FUNCTION_COUNT)
  
  randomSeed(analogRead(0));
  long n = random(1, 10) * 10000;

  volatile long x;
  for(long i(0); i < n ; ++i) {
    x = i / 2;  // just some math to take time
  }
}
