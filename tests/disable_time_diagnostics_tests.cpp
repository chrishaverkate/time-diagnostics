#include "disable_time_diagnostics_tests.h"

#include "minunit.h"

// Disable the Time Diagnostics before it's included
#define DISABLE_TIME_DIAGNOSTICS
#include "../src/time_diagnostics.h"

#include "../identities.h"

static char* test_disable_diagnostics() {

	TimeDiagnostics<2> td;
	{ START_TIMER (td, Timers::td_get_timer); }

	mu_assert("Failed, TD: no timers should have been reported.", td.number_of_reports() == 0);

	return nullptr;
}

char* disable_time_diagnostics_all_tests() {
	mu_run_test(test_disable_diagnostics);
	return nullptr;
}
