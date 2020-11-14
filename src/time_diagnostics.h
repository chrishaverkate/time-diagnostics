#pragma once
#include "timer.h"
#include "time_post_receiver.h"
#include <Arduino.h>


/** @brief Simplified wrapper to getting a new timer object.
 * 	It's recommended to use this for cleaner code and easier use.
 */
#define START_TIMER(td, id) Timer t___FILE_____LINE__ = td.get_timer(id);

/** @brief Provides a diagnostics framework for timing on the Arduino.
 * 	Create one TimeDiagnostics object for the lifetime of your related measurements and
 * 	request new timers (@ref get_timer) to time various sections.
 *
 * 	Example:
 * 	@code
TimeDiagnostics td;
{ // new scope limiter
	START_TIMER(td, Timers::timer_id);
 	// Your code section
 } // the timer will finish timing and post because object is destroyed at the end of the scope.
@endcode
 *
 * @note The template parameters are to ensure a fixed size and avoid allocations.
 *
 * @tparam length_posts the number of timer recordings you expect to have
 * @tparam length_report the length, in characters, you will need for the report (csv format).
 */
template<unsigned int length_posts, unsigned int length_report>
class TimeDiagnostics : public TimePostReceiver {
public:
	/** @brief Creates a new timer which starts the timing mechanism.
	 * 	Timing stops when the object leaves a scope and the destructor is called.
	 * @param[in] id the ID of the timer. See @ref Timers::Identity and @ref Timers::NAMES.
	 * @returns a new timer object that is tracking it's start time.
	 */
	Timer get_timer(Timers::Identity id) {
		return Timer(id, this);
	}

	/** @brief Implementation of the @ref TimePostReceiver interface and is used by each timer
	 * 	to record it's lifetime in microseconds.
	 * @param[in] time microseconds the object was alive or "in scope".
	 * @param[in] id The @ref Timers::Identity "timer ID" to record the time for.
	 */
	void post(unsigned long time, Timers::Identity id) override {
		if(_position < length_posts) {
			_timer_ids[_position] = id;
			_measurements[_position] = time;
			++_position;
		}
	}

	/** @brief Allows for accessing individual timer results based on the @ref Timers::Identity "ID".
	 * @param[in] id Timer ID
	 * @returns Measured time in microseconds or -1 if the ID was not found.
	 */
	long get_timer_measurement(Timers::Identity id) {
		long result = -1;

		for(unsigned int i(0); i < _position; ++i) {
			if(_timer_ids[i] == id) {
				result = _measurements[i];
				break;
			}
		}

		return result;
	}

	/** @brief Formats all recorded times in CSV format with the @ref Timers::NAMES "Names" and @ref Timers::Identity "IDs"
	 * @returns Pointer to char buffer, valid until next call.
	 */
	const char* report() {
		int index = sprintf(buffer_report, "Timer Name,Microseconds\n");
		for(unsigned int i(0); i < _position; ++i) {
			index += sprintf(buffer_report + index, "%s,%d\n", Timers::NAMES[_timer_ids[i]], _measurements[i]);
		}
		return buffer_report;
	}

private:
	char buffer_report[length_report]{};         ///< buffer for storing report. Populated in @ref report.
	Timers::Identity _timer_ids[length_posts]{}; ///< tracks @ref Timers::Identity "Timer IDs" that have posted results; in order of post.
	unsigned long _measurements[length_posts]{}; ///< tracks measurements recorded by each timer, in microseconds.
	unsigned int _position = 0;                  ///< position in the collections to place the next measurement. Also, current number of records.
};

