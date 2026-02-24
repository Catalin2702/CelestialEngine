//
// Created by Catalin Chirosca on 2026-02-18.
//

#pragma once

#ifndef CE_WINDOW_MAC_METALVIEWPORT_HPP
#define CE_WINDOW_MAC_METALVIEWPORT_HPP

#include "Types/Window/WindowProps.hpp"
#include "Types/Window/WindowDestructor.hpp"
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

namespace CeTypeWindow = CE::Types::Window;

namespace CE::Window {

class MetalViewport final : public InterfaceViewport {
public:
	MetalViewport(const CeTypeWindow::WindowProps& windowProps);
	~MetalViewport() override;

public:
	void OnUpdate() override;

public:
	[[nodiscard]] unsigned int GetWidth() const override { return _data.width; }
	[[nodiscard]] unsigned int GetHeight() const override { return _data.height; }
	[[nodiscard]] bool IsVSync() const override { return _data.VSync; }
	[[nodiscard]] void* GetNativeWindow() const override { return _metalWindow.get(); }

	void SetEventCallback(const EventCallbackFn& callback) override;
	void SetWindowCallbacks() override;
	void SetWidth(unsigned int width) override;
	void SetHeight(unsigned int height) override;
	void SetVSync(bool enabled) override;

private:
	void _Init();
	void _InitDevice();
	void _InitWindow();
	void _Shutdown();


private:
	EventWindowData _data;
	CeTypeWindow::GLFWwindowPtr _glfwWindow;
	NS::SharedPtr<MTL::CommandQueue> _commandQueue;
	NS::SharedPtr<MTL::Device> _metalDevice;
	NS::SharedPtr<NS::Window> _metalWindow;
	NS::SharedPtr<CA::MetalLayer> _metalLayer;
};

}


#endif //CE_WINDOW_MAC_METALVIEWPORT_HPP
