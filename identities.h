#pragma once

/** @brief Scope for managing the IDs and Names.
 */
namespace Timers {
	/** Organizing timer IDs */
	enum Identity {
		tests_all = 0
		, td_get_timer
		, td_post_overflow
		, td_another

		, tests_timer
		, tests_time_diagnostics

		, Invalid
	};

	/** Print friendly label / name for Timers. */
	static const char* NAMES[] = {
		"Tests: All"
		, "TD: get timer"
		, "TD: post overflow"
		, "TD: another timer for reporting"

		, "Tests: Timer"
		, "Tests: Time Diagnostics"

		, "Invalid"
	};
}
