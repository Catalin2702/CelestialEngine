//
// Module: Tools/Time
// File: Chronometer.hpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-16
// Updated: 2026-02-28
//

#pragma once


#ifndef CE_TOOLS_TIME_TIMER_HPP
#define CE_TOOLS_TIME_TIMER_HPP

#include <chrono>


namespace CE::Tools::Time {

class Chronometer {
#ifdef CE_DEBUG
public:
	Chronometer();
	~Chronometer();

private:
	void Start();
	void Stop();
	void PrintResult() const;

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> _start;
	std::chrono::time_point<std::chrono::high_resolution_clock> _end;
#endif
};

}

#endif //CE_TOOLS_TIME_TIMER_HPP
