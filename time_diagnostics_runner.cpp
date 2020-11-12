#include <Arduino.h>
#include "time_diagnostics.h"

#include "src/serial_printf.h"
#include "src/timer.h"
#include "src/identities.h"

#include "tests/minunit.h"
#include "tests/timer_tests.h"
#include "tests/time_diagnostics_tests.h"

int tests_run = 0;
TimeDiagnostics<10, 500> all_tests_diagnostics;

/** @brief Runs all test suites in the tests folder
 */
static char* all_tests() {
	{
		START_TIMER(all_tests_diagnostics, Timers::tests_timer);
		mu_run_test(timer_all_tests);
	}

	{
		START_TIMER(all_tests_diagnostics, Timers::tests_time_diagnostics);
		mu_run_test(time_diagnostics_all_tests);
	}
	return nullptr;
}

void setup() {
	Serial.begin(115200);
}

void loop() {
	{
		START_TIMER(all_tests_diagnostics, Timers::tests_all);

		char* result = all_tests();
		if (result != nullptr) {
			serial_printf(Serial, "%s\n", result);
		} else {
			serial_printf(Serial, "ALL TESTS PASSED\n");
		}
		serial_printf(Serial, "Tests run: %d\n", tests_run);
	}

	serial_printf(Serial
			   ,"Finished in %d micro-seconds.\n"
			   , all_tests_diagnostics.get_timer_measurement(Timers::tests_all));

	serial_printf(Serial, "\nFull Report:\n%s", all_tests_diagnostics.report());
	Serial.println("\n========================================================\n");

	while(true);
}
