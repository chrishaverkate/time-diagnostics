#include "timer.h"
#include <Arduino.h>

Timer::Timer(Timers::Identity identity, TimePostReceiver* receiver)
	: _id(identity)
	, _receiver(receiver)
	, _time_start(micros())
{

}

Timers::Identity Timer::get_identity() const {
	return _id;
}

Timer::~Timer() {
	_receiver->post(micros() - _time_start, _id);
}
