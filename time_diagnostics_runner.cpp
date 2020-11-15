#include <Arduino.h>
#include "identities.h"

#include "src/time_diagnostics.h"

#include "tests/minunit.h"
#include "tests/serial_printf.h"

#include "tests/disable_time_diagnostics_tests.h"
#include "tests/time_diagnostics_tests.h"
#include "tests/timer_tests.h"

int tests_run = 0;
TimeDiagnostics<10> all_tests_diagnostics;


/** @brief Runs all test suites in the tests folder
 */
static char* all_tests() {
	{
		START_TIMER(all_tests_diagnostics, Timers::tests_timer)
		mu_run_test(timer_all_tests);
	}

	{
		START_TIMER(all_tests_diagnostics, Timers::tests_time_diagnostics)
		mu_run_test(time_diagnostics_all_tests);
	}
	{
		mu_run_test(disable_time_diagnostics_all_tests);
	}
	return nullptr;
}

void setup() {
	Serial.begin(115200);
}

void report() {
	const unsigned int TIMER_COUNT = all_tests_diagnostics.number_of_reports();

	char buffer_report[300] = {};
	int buffer_index = sprintf(buffer_report, "Timer ID,Microseconds\n");

	for(int timer_index(0); timer_index < TIMER_COUNT; ++timer_index) {
		int id = -1;
		unsigned long micros = 0;
		if(all_tests_diagnostics.timer_report(timer_index, id, micros)) {
			buffer_index += sprintf(buffer_report + buffer_index, "%s,%d\n", Timers::NAMES[id], micros);
		}
	}
	Serial.println(buffer_report);
}

void loop() {
	Serial.println("\n========================================================\n");
	{
		START_TIMER(all_tests_diagnostics, Timers::tests_all)

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
			   , all_tests_diagnostics.timer_measurement(Timers::tests_all));

	serial_printf(Serial, "\nFull Report:\n");
	report();
	Serial.println("========================================================\n");

	while(true);
}
