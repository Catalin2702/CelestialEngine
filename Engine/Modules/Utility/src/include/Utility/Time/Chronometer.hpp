//
// Module: CelestialEngine/Engine/Modules/Utility/Time
// File: Chronometer.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-16
// Updated by: Catalin Chirosca
// Updated: 2026-08-29
//

#pragma once

#ifndef CE_UTILITY_TIME_TIMER_HPP
#define CE_UTILITY_TIME_TIMER_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Var/Vars.hpp"

#include <chrono>


namespace CE::Utility {

/**
 * @class Chronometer
 * @brief RAII-style timing utility for measuring code execution time
 * @details Automatically starts timing on construction and stops on destruction, printing the elapsed time (the
 *			printing only happens in debug builds, so a chronometer left in the code costs two clock reads in
 *			Release/Dist). The measure can also be read while the chronometer is alive, or frozen early with Stop().
 */
class CE_UTILITY_API Chronometer {
public:
	using Clock = std::chrono::high_resolution_clock;
	using TimePoint = std::chrono::time_point<Clock>;

public:
	/**
	 * @brief Constructor - starts the timer
	 * @details Captures the current high-resolution clock time as the start point
	 */
	Chronometer();

	/**
	 * @brief Destructor - stops the timer and prints results
	 * @details Captures the end time and prints the elapsed duration
	 */
	~Chronometer();

public:
	/**
	 * @brief Stops the chronometer, freezing the measured duration
	 * @details Calling it is optional: the destructor stops the chronometer anyway. Stopping an already stopped
	 *			chronometer does nothing, so an early Stop() is never overwritten by the destructor.
	 */
	void Stop();

public:
	/**
	 * @brief Gets the measured duration
	 * @return std::chrono::nanoseconds Time between the start and the stop, or between the start and now while the
	 *			chronometer is still running
	 */
	[[nodiscard]] std::chrono::nanoseconds GetElapsed() const;

	/**
	 * @brief Gets the measured duration in milliseconds
	 * @return f64 Same measure as GetElapsed(), expressed in milliseconds with the sub-millisecond fraction
	 */
	[[nodiscard]] f64 GetElapsedMilliseconds() const;

	/**
	 * @brief Checks whether the chronometer has been stopped
	 * @return bool True once the duration is frozen, false while it is still running
	 */
	[[nodiscard]] bool IsStopped() const { return _isStopped; }

private:
	/**
	 * @brief Starts the chronometer
	 * @details Records the current time as the start point
	 */
	void Start();

	/**
	 * @brief Prints the measured time duration
	 * @details Calculates and outputs the elapsed time between start and stop
	 */
	void PrintResult() const;

private:
	TimePoint _start;								///< Start time point
	TimePoint _end;									///< End time point, meaningful only once the chronometer is stopped
	bool _isStopped = false;						///< True once Stop() froze the measure, so the destructor does not overwrite it
};

}

#endif //CE_UTILITY_TIME_TIMER_HPP
