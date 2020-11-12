#include "timer_tests.h"

#include <Arduino.h>
#include "minunit.h"

#include "../src/timer.h"

#include "fake_time_receiver.h"

static char* test_timer_create() {
	FakeTimeReceiver ftr;
	Timer t = Timer(Timers::tests_all, &ftr);
	mu_assert("Failed: Wrong timer identity.",t.get_identity() == Timers::tests_all);
	return nullptr;
}

static char* test_timer_post_time() {
	FakeTimeReceiver ftr;
	{
		Timer t = Timer(Timers::tests_all, &ftr);
		delayMicroseconds(10);
	}
	mu_assert("Failed: No time passed.",ftr._time >= 10U);
	mu_assert("Failed: Invalid timer ID.",ftr._id != Timers::Invalid);
	return nullptr;
}

char* timer_all_tests() {
	mu_run_test(test_timer_create);
	mu_run_test(test_timer_post_time);
	return nullptr;
}
