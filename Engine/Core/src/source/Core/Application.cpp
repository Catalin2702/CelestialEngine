//
// Created by Catalin Chirosca on 2026-02-15.
//

#include "Core/Application.hpp"
#include <print>

namespace CE::Core {

Application::Application() {
	std::println("Application created");
}

Application::~Application() {
	std::println("Application destructed");
}

void Application::Run() {
	std::println("Start Run");
	int loop = 20;
	while (loop > 0)
		loop--;
	std::println("Stop Run");
}

}
