//
// Created by Catalin Chirosca on 2026-02-18.
//

#pragma once

#ifndef CE_WINDOW_MAC_METALVIEWPORT_HPP
#define CE_WINDOW_MAC_METALVIEWPORT_HPP

#include "Utils/Window.hpp"
#include "Window/InterfaceWindow.hpp"

#include <memory>
#include <AppKit/AppKit.hpp>
#include <Foundation/Foundation.hpp>


struct GLFWwindow;
namespace CA {
class MetalLayer;
}
namespace MTL {
class CommandQueue;
class Device;
}
namespace NS {
class Window;
}

namespace CE::Window {

class MetalViewport final : public InterfaceViewport {
public:
	MetalViewport(const WindowProps& windowProps);
	~MetalViewport() override;

public:
	void OnUpdate() override;

public:
	[[nodiscard]] unsigned int GetWidth() const override { return _data.width; }
	[[nodiscard]] unsigned int GetHeight() const override { return _data.height; }
	[[nodiscard]] bool IsVSync() const override { return _data.VSync; }

	void SetEventCallback(const EventCallbackFn& callback) override;
	void SetWindowCallbacks() override;
	void SetWidth(unsigned int width) override;
	void SetHeight(unsigned int height) override;
	void SetVSync(bool enabled) override;

private:
	void _Init(const WindowProps& windowProps);
	void _InitDevice();
	void _InitWindow();
	void _Shutdown();


private:
	WindowData _data;
	NS::SharedPtr<MTL::CommandQueue> _commandQueue;
	GLFWWindowPtr _glfwWindow;
	NS::SharedPtr<MTL::Device> _metalDevice;
	NS::SharedPtr<NS::Window> _metalWindow;
	NS::SharedPtr<CA::MetalLayer> _metalLayer;
};

}


#endif //CE_WINDOW_MAC_METALVIEWPORT_HPP
