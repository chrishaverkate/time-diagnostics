#pragma once

#include "../src/internal/time_post_receiver.h"

class FakeTimeReceiver : public TimePostReceiver {
public:
	FakeTimeReceiver()
		: _time(0U)
		, _id(-1)
	{}
	void post(unsigned long time, int id) override {
		_time = time;
		_id = id;
	}

	unsigned long _time;
	int _id;
};
