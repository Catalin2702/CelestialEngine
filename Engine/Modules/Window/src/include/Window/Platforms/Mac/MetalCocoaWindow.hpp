//
// Module: CelestialEngine/Engine/Modules/Window/Platforms/Mac
// File: MetalCocoaWindow.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-16
// Updated by: Catalin Chirosca
// Updated: 2026-03-17
//

#pragma once

#ifndef CE_WINDOW_MAC_METALCOCOAWINDOW_HPP
#define CE_WINDOW_MAC_METALCOCOAWINDOW_HPP

#include "Types/Window/WindowProps.hpp"
#include "Window/I_Window.hpp"

#include <AppKit/AppKit.hpp>
#include <CoreData/CoreData.h>
#include <Foundation/Foundation.hpp>

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


namespace TypeWindow = CE::Types::Window;

namespace CE::Window {

class MetalCocoaWindow final: public I_Window {
public:
	MetalCocoaWindow(const TypeWindow::WindowProps& windowProps);
	~MetalCocoaWindow() override;

public:
	void OnUpdate() const override;

public:
	[[nodiscard]] unsigned int GetWidth() const override { return _data.width; }
	[[nodiscard]] unsigned int GetHeight() const override { return _data.height; }
	[[nodiscard]] bool IsVSync() const override { return _data.VSync; }
	[[nodiscard]] void* GetNativeWindow() const override { return GetMetalWindow(); }

	[[nodiscard]] MTL::CommandQueue* GetCommandQueue() const { return _commandQueue.get(); }
	[[nodiscard]] MTL::Device* GetDevice() const { return _device.get(); }
	[[nodiscard]] CA::MetalLayer* GetMetalLayer() const { return _layer.get(); }
	[[nodiscard]] NS::Window* GetMetalWindow() const { return _window; }

public:
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
	void _UpdateLayerSize() const;

private:
	EventWindowData _data;							///< Window data including event callback

	NS::SharedPtr<MTL::CommandQueue> _commandQueue;	///< Metal command queue
	NS::SharedPtr<MTL::Device> _device;				///< Metal device (GPU)
	NS::SharedPtr<CA::MetalLayer> _layer;			///< Core Animation Metal layer
	NS::Window* _window;							///< Native macOS window
	void* _windowDelegate;							///< Cocoa window delegate for handling events
};

}

#endif //CE_WINDOW_MAC_METALCOCOAWINDOW_HPP
