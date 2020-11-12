#pragma once
/** @file identities.h
 * @brief User populated timer identities.
 * 	These @ref Timers::Identity "Identities" are what the user can reference when creating new timers.
 * 	The @ref Timers::NAMES array is for providing print friendly labels to the IDs in the enum.
 *
 * 	@warning The value of @ref Timers::Identity is used as an index into @ref Timers::NAMES.
 * 	It's key that these are kept in matching order. Otherwise, the report is going to be a mess.
 */

/** @brief Scope for managing the IDs and Names.
 */
namespace Timers {
	/** @brief Used in calls to @ref TimeDiagnostics::get_timer to supply an ID for the timer.
	 */
	enum Identity {
		tests_all = 0
		, td_get_timer
		, td_post_overflow
		, td_another

		, tests_timer
		, tests_time_diagnostics



		, Invalid
	};

	/** @brief Print friendly label / name for Timers.
	 */
	static const char* NAMES[] = {
		"All Tests"
		, "TD: get timer"
		, "TD: post overflow"
		, "TD: another timer for reporting"

		, "Timer Tests"
		, "Time Diagnostics Tests"

		, "Invalid"
	};
}
