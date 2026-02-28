//
// Created by Catalin Chirosca on 2026-02-18.
//

#pragma once

#ifndef CE_WINDOW_MAC_METALVIEWPORT_HPP
#define CE_WINDOW_MAC_METALVIEWPORT_HPP

#include "Types/Window/WindowDestructor.hpp"
#include "Types/Window/WindowProps.hpp"
#include "Window/I_Viewport.hpp"

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

class MetalViewport final : public I_Viewport {
public:
	MetalViewport(const CeTypeWindow::WindowProps& windowProps);
	~MetalViewport() override;

public:
	void OnUpdate() override;

public:
	[[nodiscard]] unsigned int GetWidth() const override { return _data.width; }
	[[nodiscard]] unsigned int GetHeight() const override { return _data.height; }
	[[nodiscard]] bool IsVSync() const override { return _data.VSync; }
	[[nodiscard]] GLFWwindow* GetGLFWwindow() const override { return _glfwWindow.get(); }
	[[nodiscard]] MTL::CommandQueue* GetCommandQueue() const { return _commandQueue.get(); }
	[[nodiscard]] MTL::Device* GetDevice() const { return _metalDevice.get(); }
	[[nodiscard]] CA::MetalLayer* GetMetalLayer() const { return _metalLayer.get(); }
	[[nodiscard]] NS::Window* GetMetalWindow() const { return _metalWindow.get(); }


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
	NS::SharedPtr<CA::MetalLayer> _metalLayer;
	NS::SharedPtr<NS::Window> _metalWindow;
};

}


#endif //CE_WINDOW_MAC_METALVIEWPORT_HPP
