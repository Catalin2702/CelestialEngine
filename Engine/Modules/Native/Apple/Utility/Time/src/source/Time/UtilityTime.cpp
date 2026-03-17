//
// Module: CelestialEngine/Engine/Native/Apple/Utility/Time
// File: Time.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-17
// Updated by: Catalin Chirosca
// Updated: 2026-03-17
//

#include "Time/UtilityTime.hpp"

#include <mach/mach_time.h>

namespace CE::Apple::Utility {

float GetTime() {
	static mach_timebase_info_data_t timebase;
	if (timebase.denom == 0) mach_timebase_info(&timebase);

	const uint64_t time = mach_absolute_time();
	// Converte in nanosecondi e poi in secondi
	return static_cast<float>(static_cast<double>(time) * timebase.numer / timebase.denom) / 1e9f;
}

}