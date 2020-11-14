#include "time_diagnostics_tests.h"

#include <Arduino.h>
#include "minunit.h"

#include "../src/timer.h"
#include "../src/time_diagnostics.h"

static char* test_time_diag_create() {
	TimeDiagnostics<2, 100> td;
	mu_assert("Failed, TD: could not create.",td.get_timer_measurement(Timers::Invalid) == -1);
	return nullptr;
}

static char* test_time_diag_get_timer() {
	TimeDiagnostics<2, 100> td;
	{
		Timer t = td.get_timer(Timers::td_get_timer);
		delayMicroseconds(8);
	}

	mu_assert("Failed, TD: timer measurement wrong.",td.get_timer_measurement(Timers::td_get_timer) >= 8);
	return nullptr;
}

static char* test_time_diag_timer_macro() {
	TimeDiagnostics<2, 100> td;
	{
		START_TIMER (td, Timers::td_get_timer);
		delayMicroseconds(8);
	}

	mu_assert("Failed, TD: timer measurement wrong.",td.get_timer_measurement(Timers::td_get_timer) >= 8);
	return nullptr;
}

static char* test_handle_post_overflow() {
	TimeDiagnostics<1, 100> td;
	{
		Timer t1 = td.get_timer(Timers::td_get_timer);
		delayMicroseconds(8);
	}

	{
		Timer t2 = td.get_timer(Timers::td_post_overflow);
	}

	long actual_t1 = td.get_timer_measurement(Timers::td_get_timer);
	long actual_t2 = td.get_timer_measurement(Timers::td_post_overflow);

	mu_assert("Failed, TD: didn't get first timer",actual_t1 >= 8);
	mu_assert("Failed, TD: second timer returned valid measurement", actual_t2 == -1);

	return nullptr;
}

static char* test_get_report_single_entry() {
	TimeDiagnostics<1, 100> td;
	{
		Timer t = td.get_timer(Timers::td_get_timer);
	}

	const char* report = td.report();
	mu_assert("Failed, TD: report was nullptr.", report != nullptr);
	mu_assert("Failed, TD: report is empty.", report[0] != '\0');

	int newlines = 0;
	for(size_t i(0); i < strlen(report); ++i) {
		if(report[i] == '\n') {
			++newlines;
		}
	}
	mu_assert("Failed, TD: wrong number of entries in report", newlines == 2);

	return nullptr;
}

static char* test_get_report_multiple_entry() {
	TimeDiagnostics<2, 100> td;
	{ Timer t1 = td.get_timer(Timers::td_get_timer); }
	{ Timer t2 = td.get_timer(Timers::td_another); }

	const char* report = td.report();
	mu_assert("Failed, TD: report was nullptr.", report != nullptr);
	mu_assert("Failed, TD: report is empty.", report[0] != '\0');

	int newlines = 0;
	for(size_t i(0); i < strlen(report); ++i) {
		if(report[i] == '\n') {
			++newlines;
		}
	}

	mu_assert("Failed, TD: didn't find enough entries for multiple timers", newlines == 3);

	return nullptr;
}

char* time_diagnostics_all_tests() {
	mu_run_test(test_time_diag_create);
	mu_run_test(test_time_diag_get_timer);
	mu_run_test(test_time_diag_timer_macro);
	mu_run_test(test_handle_post_overflow);
	mu_run_test(test_get_report_single_entry);
	mu_run_test(test_get_report_multiple_entry);
	return nullptr;
}
