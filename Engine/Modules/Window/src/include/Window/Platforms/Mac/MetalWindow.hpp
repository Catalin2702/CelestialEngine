//
// Module: Window/Platforms/Mac
// File: MetalWindow.hpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-18
// Updated: 2026-02-28
//

#pragma once

#ifndef CE_WINDOW_MAC_METALWINDOW_HPP
#define CE_WINDOW_MAC_METALWINDOW_HPP

#include "Types/Window/WindowDestructor.hpp"
#include "Types/Window/WindowProps.hpp"
#include "Window/I_Window.hpp"

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

namespace TypeWindow = CE::Types::Window;

namespace CE::Window {

/**
 * @class MetalWindow
 * @brief macOS-specific window implementation using Metal API
 * @details Provides a window implementation for macOS that uses Metal for rendering.
 *			Manages both GLFW window for input handling and Metal resources for graphics.
 *			This class integrates GLFW's cross-platform windowing with Apple's Metal API.
 */
class MetalWindow final: public I_Window {
public:
	/**
	 * @brief Constructor
	 * @param windowProps Window configuration properties (title, dimensions, VSync)
	 * @details Creates and initializes a Metal-based window with the specified properties
	 */
	MetalWindow(const TypeWindow::WindowProps& windowProps);

	/**
	 * @brief Destructor
	 * @details Cleans up Metal and GLFW resources
	 */
	~MetalWindow() override;

public:
	/**
	 * @brief Updates the window state each frame
	 * @details Polls events and handles buffer swapping for the Metal surface
	 */
	void OnUpdate() const override;

public:
	/**
	 * @brief Gets the current width of the window
	 * @return unsigned int Width in pixels
	 */
	[[nodiscard]] unsigned int GetWidth() const override { return _data.width; }

	/**
	 * @brief Gets the current height of the window
	 * @return unsigned int Height in pixels
	 */
	[[nodiscard]] unsigned int GetHeight() const override { return _data.height; }

	/**
	 * @brief Checks if VSync is enabled
	 * @return bool True if VSync is enabled
	 */
	[[nodiscard]] bool IsVSync() const override { return _data.VSync; }

	/**
	 * @brief Gets the underlying GLFW window pointer
	 * @return GLFWwindow* Pointer to the GLFW window
	 */
	[[nodiscard]] GLFWwindow* GetGLFWwindow() const override { return _glfwWindow.get(); }

	/**
	 * @brief Gets the Metal command queue
	 * @return MTL::CommandQueue* Pointer to the Metal command queue
	 * @details The command queue is used to submit rendering commands to the GPU
	 */
	[[nodiscard]] MTL::CommandQueue* GetCommandQueue() const { return _commandQueue.get(); }

	/**
	 * @brief Gets the Metal device
	 * @return MTL::Device* Pointer to the Metal device
	 * @details The device represents the GPU and is used to create Metal resources
	 */
	[[nodiscard]] MTL::Device* GetDevice() const { return _metalDevice.get(); }

	/**
	 * @brief Gets the Metal layer
	 * @return CA::MetalLayer* Pointer to the Core Animation Metal layer
	 * @details The Metal layer is the rendering surface for Metal content
	 */
	[[nodiscard]] CA::MetalLayer* GetMetalLayer() const { return _metalLayer.get(); }

	/**
	 * @brief Gets the native macOS window
	 * @return NS::Window* Pointer to the AppKit window
	 * @details Provides access to the underlying macOS window for platform-specific operations
	 */
	[[nodiscard]] NS::Window* GetMetalWindow() const { return _metalWindow; }

	/**
	 * @brief Sets the event callback function
	 * @param callback Function to be called when events occur
	 */
	void SetEventCallback(const EventCallbackFn& callback) override;

	/**
	 * @brief Configures all window event callbacks
	 * @details Sets up GLFW callbacks for window and input events
	 */
	void SetWindowCallbacks() override;

	/**
	 * @brief Sets the window width
	 * @param width New width in pixels
	 */
	void SetWidth(unsigned int width) override;

	/**
	 * @brief Sets the window height
	 * @param height New height in pixels
	 */
	void SetHeight(unsigned int height) override;

	/**
	 * @brief Enables or disables VSync
	 * @param enabled True to enable VSync, false to disable
	 */
	void SetVSync(bool enabled) override;

private:
	/**
	 * @brief Initializes the Metal window
	 * @details Calls _InitDevice() and _InitWindow() in sequence
	 */
	void _Init();

	/**
	 * @brief Initializes the Metal device and command queue
	 * @details Creates the default Metal device and a command queue for rendering
	 */
	void _InitDevice();

	/**
	 * @brief Initializes the window and Metal layer
	 * @details Creates the GLFW window, retrieves the native macOS window,
	 *			and sets up the Metal layer for rendering
	 */
	void _InitWindow();

	/**
	 * @brief Cleans up Metal and GLFW resources
	 * @details Releases all Metal resources and destroys the GLFW window
	 */
	void _Shutdown();

private:
	EventWindowData _data;										///< Window data including event callback
	TypeWindow::GLFWwindowPtr _glfwWindow = nullptr;			///< GLFW window smart pointer
	NS::SharedPtr<MTL::CommandQueue> _commandQueue = nullptr;	///< Metal command queue
	NS::SharedPtr<MTL::Device> _metalDevice = nullptr;			///< Metal device (GPU)
	NS::SharedPtr<CA::MetalLayer> _metalLayer = nullptr;		///< Core Animation Metal layer
	NS::Window* _metalWindow = nullptr;							///< Native macOS window
};

}

#endif //CE_WINDOW_MAC_METALWINDOW_HPP
