//
// Module: CelestialEngine/Engine/Modules/Render/Context
// File: I_Context.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-19
// Updated by: Catalin Chirosca
// Updated: 2026-05-19
//

#pragma once

#ifndef CE_RENDER_CONTEXT_CONTEXT_HPP
#define CE_RENDER_CONTEXT_CONTEXT_HPP

#include "Types/Render/Render.hpp"

#include <utility>

/**
 * @namespace CE::Core::Render::Context
 * @brief Graphics context management and interfaces
 * @details Contains the I_Context interface and related type definitions for graphics context management.
 *			This namespace defines the common interface for all graphics context implementations,
 *			allowing for platform-specific rendering contexts while maintaining a consistent API.
 *			Implementations include OpenGlContext for cross-platform OpenGL contexts and
 *			MetalContext for macOS Metal-based contexts.
 */
namespace CE::Core::Render::Context {

/**
 * @class I_Context
 * @brief Interface for graphics context management
 * @details Defines the common interface for all graphics context implementations, allowing for platform-specific rendering contexts while maintaining a consistent API.
 */
class I_Context {
public:
	/**
	 * @brief Virtual destructor
	 * @details Ensures proper cleanup of derived context implementations
	 */
	virtual ~I_Context() = default;

public:
	/**
	 * @brief Initializes the graphics context
	 * @details Pure virtual method that initializes the graphics context, sets up necessary resources,
	 *			and prepares it for rendering. Must be implemented by derived classes.
	 */
	virtual void Init() = 0;
	/**
	 * @brief Swaps the front and back buffers
	 * @details Pure virtual method that handles buffer swapping to present the rendered frame on the screen.
	 *			Must be implemented by derived classes to ensure proper presentation of rendered content.
	 */
	virtual void SwapBuffers() = 0;

public:
	/**
	 * @brief Handles changes in content size (e.g., when the window is resized)
	 * @param size New content size as a pair of floats (width, height)
	 * @details Pure virtual method that updates the graphics context's settings when the content size changes, such as during window resizing.
	 *			This ensures that rendering remains correct at different sizes and that the viewport and related settings are updated accordingly.
	 */
	virtual void HandleContentSizeChange(const std::pair<float, float>& size) = 0;

	/**
	 * @brief Handles changes in VSync state
	 * @param enabled True if VSync is enabled, false if disabled
	 * @details Pure virtual method that updates the graphics context's settings when the VSync state changes.
	 *			This allows the context to enable or disable synchronization with the monitor's refresh rate as needed.
	 */
	virtual void HandleVSyncChange(bool enabled) = 0;

public:
	/**
	 * @brief Checks if VSync is currently enabled
	 * @return bool True if VSync is enabled, false otherwise
	 * @details Pure virtual method that returns the current state of VSync in the graphics context. This allows the application to query whether VSync is active and adjust rendering behavior accordingly.
	 */
	[[nodiscard]] virtual bool IsVSyncEnabled() const = 0;

public:
	/**
	 * @brief Gets the native graphics device or context handle
	 * @return void* Pointer to the native graphics device or context (e.g., OpenGL context, Metal device)
	 * @details Pure virtual method that returns a pointer to the underlying native graphics device or context.
	 *			This allows the application to access platform-specific features or perform operations that require direct access to the native context.
	 */
	[[nodiscard]] virtual void* GetNativeDevice() const = 0;

	/**
	 * @brief Gets the native rendering layer or surface handle
	 * @return void* Pointer to the native rendering layer or surface (e.g., Metal layer, OpenGL framebuffer)
	 * @details Pure virtual method that returns a pointer to the underlying native rendering layer or surface.
	 *			This allows the application to access platform-specific features or perform operations that require direct access to the rendering target.
	 */
	[[nodiscard]] virtual void* GetNativeLayer() const = 0;

	/**
	 * @brief Gets the native command queue or command list handle
	 * @return void* Pointer to the native command queue or command list (e.g., Metal command queue, OpenGL command list)
	 * @details Pure virtual method that returns a pointer to the underlying native command queue or command list used for issuing rendering commands.
	 *			This allows the application to access platform-specific features or perform operations that require direct access to the command submission mechanism.
	 */
	[[nodiscard]] virtual void* GetNativeCommandQueue() const = 0;

public:
	/**
	 * @brief Gets the graphics API used by this context
	 * @return Types::Render::GraphicsApi Enum value representing the graphics API (e.g., OpenGL, Metal)
	 * @details Pure virtual method that returns the graphics API type used by this context.
	 *			This allows the application to query the context for its underlying graphics API, which can be useful for conditional rendering logic or debugging.
	 */
	[[nodiscard]] virtual Types::Render::GraphicsApi GetGraphicsApi() const = 0;
};

}

#endif //CE_RENDER_CONTEXT_CONTEXT_HPP
