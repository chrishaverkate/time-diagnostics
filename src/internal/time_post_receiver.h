#pragma once

/** @brief An abstract interface for defining how timer data can be posted.
 */
class TimePostReceiver {
public:
	virtual void post(unsigned long time, int id) = 0;
};
