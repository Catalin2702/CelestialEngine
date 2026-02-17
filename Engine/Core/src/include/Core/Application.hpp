//
// Created by Catalin Chirosca on 2026-02-15.
//

#pragma once

#ifndef CE_CORE_APPLICATION_HPP
#define CE_CORE_APPLICATION_HPP

#include "Define/DynamicLinker.hpp"
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
#endif //CE_CORE_APPLICATION_HPP
