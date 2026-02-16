//
// Created by Catalin Chirosca on 2026-02-15.
//

#pragma once

#ifndef CELESTIALENGINE_APPLICATION_HPP
#define CELESTIALENGINE_APPLICATION_HPP

#include "Core/Core.hpp"
#include "Events/Event.hpp"

#include <memory>


namespace CE::Window {

class Window;

}


namespace CE::Core {

class CE_API Application {
public:
	Application();

	virtual ~Application();

public:
	virtual void Run();

private:
	std::unique_ptr<Window::Window> _window;
	bool _running;
};

Application *CreateApplication();

}

#endif //CELESTIALENGINE_APPLICATION_HPP
