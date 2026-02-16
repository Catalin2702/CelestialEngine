//
// Created by Catalin Chirosca on 2026-02-16.
//

#pragma once


#ifndef WORKSPACE_TIMER_HPP
#define WORKSPACE_TIMER_HPP

#include <chrono>


namespace CE::Tools::Time {

class Timer {
public:
	Timer();
	~Timer();

private:
	void Start();
	void Stop();
	void PrintResult();

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> _start;
	std::chrono::time_point<std::chrono::high_resolution_clock> _end;
};

}

#endif //WORKSPACE_TIMER_HPP
