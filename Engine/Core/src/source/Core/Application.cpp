//
// Created by Catalin Chirosca on 2026-02-15.
//
#include <Core/Application.hpp>
#include <print>

namespace CE::Core {
	Application::Application() {

	}

	Application::~Application() {

	}

	void Application::Run() {
		std::println("Start Run");
		while (true);
		std::println("Stop Run");
	}
}
