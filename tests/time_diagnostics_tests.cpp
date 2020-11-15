#include "time_diagnostics_tests.h"

#include <Arduino.h>
#include "minunit.h"

#include "../identities.h"
#include "../src/internal/timer.h"
#include "../src/time_diagnostics.h"

static char* test_create() {
	TimeDiagnostics<2> td;
	mu_assert("Failed, TD: could not create.", td.timer_measurement(Timers::Invalid) == -1);
	return nullptr;
}

static char* test_get_timer() {
	TimeDiagnostics<2> td;
	{
		Timer t = td.get_timer(Timers::td_get_timer);
		delayMicroseconds(8);
	}

	mu_assert("Failed, TD: timer measurement wrong.", td.timer_measurement(Timers::td_get_timer) >= 8);
	return nullptr;
}

static char* test_get_number_of_posts() {
	TimeDiagnostics<2> td;
	{
		Timer t = td.get_timer(Timers::td_get_timer);
		delayMicroseconds(8);
	}
	{ Timer t = td.get_timer(Timers::td_get_timer); }

	mu_assert("Failed, TD: wrong number of reports.", td.number_of_reports() == 2);
	return nullptr;
}

static char* test_timer_macro() {
	TimeDiagnostics<2> td;
	{
		START_TIMER (td, Timers::td_get_timer);
		delayMicroseconds(8);
	}

	mu_assert("Failed, TD: timer measurement wrong.", td.timer_measurement(Timers::td_get_timer) >= 8);
	return nullptr;
}

static char* test_handle_post_overflow() {
	TimeDiagnostics<1> td;
	{
		Timer t1 = td.get_timer(Timers::td_get_timer);
		delayMicroseconds(8);
	}

	{
		Timer t2 = td.get_timer(Timers::td_post_overflow);
	}

	long actual_t1 = td.timer_measurement(Timers::td_get_timer);
	long actual_t2 = td.timer_measurement(Timers::td_post_overflow);

	mu_assert("Failed, TD: didn't get first timer.",actual_t1 >= 8);
	mu_assert("Failed, TD: second timer returned valid measurement.", actual_t2 == -1);

	return nullptr;
}

static char* test_get_single_timer_report() {
	TimeDiagnostics<2> td;
	{ START_TIMER (td, Timers::td_get_timer); }

	int id = -1;
	unsigned long micros = 0;
	mu_assert("Failed, TD: invalid single timer.", td.timer_report(0, id, micros));
	mu_assert("Failed, TD: invalid ID from single timer.", id == Timers::td_get_timer);
	mu_assert("Failed, TD: invalid micros from single timer.", micros > 0);
	return nullptr;
}

static char* test_get_timer_report_invalid_id_negative() {
	TimeDiagnostics<2> td;
	int id = -1;
	unsigned long micros = 0;
	mu_assert("Failed, TD: report successful for negative index", td.timer_report(-1, id, micros) == false);
	mu_assert("Failed, TD: valid ID for negative index.", id == -1);
	mu_assert("Failed, TD: valid micros for negative index.", micros == 0);
	return nullptr;
}

static char* test_get_timer_report_invalid_id_beyond_max_reports() {
	TimeDiagnostics<2> td;
	{ START_TIMER (td, Timers::td_get_timer); }
	int id = -1;
	unsigned long micros = 0;
	mu_assert("Failed, TD: report successful for invalid index", td.timer_report(1, id, micros) == false);
	mu_assert("Failed, TD: valid ID for invalid index.", id == -1);
	mu_assert("Failed, TD: valid micros for invalid index.", micros == 0);
	return nullptr;
}

static char* test_reset() {
	TimeDiagnostics<2> td;
	{ START_TIMER (td, Timers::td_get_timer); }

	mu_assert("Failed, TD: no timer posted before reset.", td.number_of_reports() == 1);

	td.reset();
	mu_assert("Failed, TD: reset didn't clear previous timers.", td.number_of_reports() == 0);

	return nullptr;
}

char* time_diagnostics_all_tests() {
	mu_run_test(test_create);
	mu_run_test(test_get_timer);
	mu_run_test(test_get_number_of_posts);
	mu_run_test(test_timer_macro);
	mu_run_test(test_handle_post_overflow);
	mu_run_test(test_get_single_timer_report);
	mu_run_test(test_get_timer_report_invalid_id_negative);
	mu_run_test(test_get_timer_report_invalid_id_beyond_max_reports);
	mu_run_test(test_reset);
	return nullptr;
}
