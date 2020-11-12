#pragma once

#include "../src/time_post_receiver.h"
#include "../src/identities.h"

class FakeTimeReceiver : public TimePostReceiver {
public:
	FakeTimeReceiver()
		: _time(0U)
		, _id(Timers::Invalid)
	{}
	void post(unsigned long time, Timers::Identity id) override {
		_time = time;
		_id = id;
	}

	unsigned long _time;
	Timers::Identity _id;
};
