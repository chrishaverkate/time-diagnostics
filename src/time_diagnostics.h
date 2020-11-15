#pragma once
#include "internal/timer.h"
#include "internal/time_post_receiver.h"
#include <Arduino.h>


#ifndef DISABLE_TIME_DIAGNOSTICS
	/** @brief Simplified wrapper to getting a new timer object.
	 * 	It's recommended to use this for cleaner code and easier use.
	 */
	#define START_TIMER(td, id) Timer t___FILE_____LINE__ = td.get_timer(id);
#else
	#define START_TIMER(td, id) ;
#endif

/** @brief Provides a diagnostics framework for timing on the Arduino.
 * 	Create one TimeDiagnostics object for the lifetime of your related measurements and
 * 	request new timers (@ref get_timer) to time various sections.
 *
 * 	Example:
 * 	@code
TimeDiagnostics td;
{ // new scope limiter
	START_TIMER(td, timer_id);
 	// Your code section
 } // the timer will finish timing and post because object is destroyed at the end of the scope.
@endcode
 *
 * @note The template parameters are to ensure a fixed size and avoid allocations.
 *
 * @tparam length_posts the number of timer recordings you expect to have
 */
template<unsigned int length_posts>
class TimeDiagnostics : public TimePostReceiver {
public:
	/** @brief Creates a new timer which starts the timing mechanism.
	 * 	Timing stops when the object leaves a scope and the destructor is called.
	 * @param[in] id the ID of the timer.
	 * @returns a new timer object that is tracking it's start time.
	 */
	Timer get_timer(int id) {
		return Timer(id, this);
	}

	/** @brief Implementation of the @ref TimePostReceiver interface and is used by each timer
	 * 	to record it's lifetime in microseconds.
	 * @param[in] time microseconds the object was alive or "in scope".
	 * @param[in] id The ID to record the time for.
	 */
	void post(unsigned long time, int id) override {
		if(_position < length_posts) {
			_timer_ids[_position] = id;
			_measurements[_position] = time;
			++_position;
		}
	}

	/** @brief Allows for accessing individual timer results based on the ID.
	 * @param[in] id Timer ID
	 * @returns Measured time in microseconds or -1 if the ID was not found.
	 */
	long timer_measurement(int id) {
		long result = -1;

		for(unsigned int i(0); i < _position; ++i) {
			if(_timer_ids[i] == id) {
				result = _measurements[i];
				break;
			}
		}

		return result;
	}

	/** @returns number of timers that have reported a time measurement
	 */
	unsigned int number_of_reports() {
		return _position;
	}

	/** @brief Provides the a timer ID and recorded measurement in microseconds.
	 * @param[in] index into the recorded measurements. Use @ref number_of_reports to know the max timers reported so far.
	 * @param[out] id filled in with the ID of the timer that posted the indexed time.
	 * @param[out] microseconds filled in with the measurement of the indexed timer.
	 * @returns false if index is invalid.
	 */
	bool timer_report(int index, int& id, unsigned long& microseconds) {
		bool valid_index = -1 < index && index < _position;

		if(valid_index) {
			id = _timer_ids[index];
			microseconds = _measurements[index];
		}

		return valid_index;
	}

	void reset() {
		_position = 0;
	}

private:
	int _timer_ids[length_posts]{};              ///< tracks Timer IDs that have posted results; in order of post.
	unsigned long _measurements[length_posts]{}; ///< tracks measurements recorded by each timer, in microseconds.
	unsigned int _position = 0;                  ///< position in the collections to place the next measurement. Also, current number of records.
};

