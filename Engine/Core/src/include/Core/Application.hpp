//
// Created by Catalin Chirosca on 2026-02-15.
//

#pragma once

#ifndef CE_CORE_APPLICATION_HPP
#define CE_CORE_APPLICATION_HPP

#include "Define/DynamicLinker.hpp"
#include "Layers/LayerStack.hpp"
#include "Types/Window/WindowProps.hpp"

#include <memory>


namespace CE {

namespace Events {
class I_Event;
class WindowCloseEvent;
}
namespace Layers {
class I_Layer;
}
namespace Window {
class I_Viewport;
}

namespace CeTypeWindow = Types::Window;

namespace Core {

class CE_API Application {
public:
	Application();
	Application(const CeTypeWindow::WindowProps& windowProps);
	Application(const std::string& title, unsigned int width, unsigned int height, bool VSync);

	virtual ~Application();

public:
	virtual void Run();
	virtual void OnEvent(Events::I_Event& event);
	virtual bool OnWindowClose(const Events::WindowCloseEvent& event);

	void PushLayer(Layers::I_Layer* layer);
	void PushOverlay(Layers::I_Layer* overlay);

public:
	[[nodiscard]] static Application& Get() { return *_instance; }
	[[nodiscard]] Window::I_Viewport* GetViewport() const { return _viewport.get(); }

protected:
	void _Init(const CeTypeWindow::WindowProps& windowProps);

private:
	std::unique_ptr<Window::I_Viewport> _viewport;
	bool _running = false;
	Layers::LayerStack _layerStack;
	static Application* _instance;
};

Application* CreateApplication();
Application* CreateApplication(const CeTypeWindow::WindowProps& windowProps);
Application* CreateApplication(const std::string& title, unsigned int width, unsigned int height, bool VSync);

}

}

#endif //CE_CORE_APPLICATION_HPP
