#pragma once
#include "identities.h"

/** @brief An abstract interface for defining how timer data can be posted.
 */
class TimePostReceiver {
public:
	virtual void post(unsigned long time, Timers::Identity id) = 0;
};
