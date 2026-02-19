//
// Created by Catalin Chirosca on 2026-02-15.
//

#pragma once

#ifndef CE_CORE_APPLICATION_HPP
#define CE_CORE_APPLICATION_HPP

#include "Define/DynamicLinker.hpp"

#include <memory>


namespace CE {

namespace Events {
class Event;
class WindowCloseEvent;
}
namespace Window {
class InterfaceViewport;
}

namespace Core {

class CE_API Application {
public:
	Application();

	virtual ~Application() = default;

public:
	virtual void Run();
	virtual void OnEvent(Events::Event& event);
	virtual bool OnWindowClose(const Events::WindowCloseEvent& event);

private:
	std::unique_ptr<Window::InterfaceViewport> _window;
	bool _running;
};

Application *CreateApplication();

}

}

#endif //CE_CORE_APPLICATION_HPP
