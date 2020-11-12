#pragma once

#include "identities.h"
#include "time_post_receiver.h"

/** @brief A basic, scope based, timing mechanism.
 * 	On construction, the current system time is recorded. When the object is destructed it captures the system time
 * 	again and posts the total run time to the @ref _receiver.
 */
class Timer {
public:
	/** @brief Constructs a new timer with an @ref Timers::Identity "ID" and an interface to post it's recorded time.
	 * @param[in] identity of the timer.
	 * @param[in] receiver to post the time to.
	 */
	explicit Timer(Timers::Identity identity, TimePostReceiver* receiver);

	/** @brief calculates lifetime of the object posts it to the @ref _receiver */
	~Timer();

	/** @brief An easy accessor for check the timer's @ref Timers::Identity "ID".
	 * @return an ID.
	 */
	Timers::Identity get_identity() const;

private:
	Timers::Identity _id;        ///< ID of the timer
	TimePostReceiver* _receiver; ///< object to send time result to
	unsigned long _time_start;   ///< time when object was created
};

