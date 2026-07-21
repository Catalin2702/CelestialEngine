//
// Module: CelestialEngine/Engine/Modules/Core/Render/Context
// File: I_Context.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-19
// Updated by: Catalin Chirosca
// Updated: 2026-07-21
//

#pragma once

#ifndef CE_CORE_RENDER_CONTEXT_CONTEXT_HPP
#define CE_CORE_RENDER_CONTEXT_CONTEXT_HPP

#include "Types/Render/Render.hpp"

#include <utility>

namespace CE::Core {

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

	virtual void SetVSync(bool enabled) = 0;

public:
	/**
	 * @brief Checks if VSync is currently enabled
	 * @return bool True if VSync is enabled, false otherwise
	 * @details Pure virtual method that returns the current state of VSync in the graphics context. This allows the application to query whether VSync is active and adjust rendering behavior accordingly.
	 */
	[[nodiscard]] virtual bool IsVSyncEnabled() const = 0;

public:
	/**
	 * @brief Gets the graphics API used by this context
	 * @return Types::Render::GraphicsApi Enum value representing the graphics API (e.g., OpenGL, Metal)
	 * @details Pure virtual method that returns the graphics API type used by this context.
	 *			This allows the application to query the context for its underlying graphics API, which can be useful for conditional rendering logic or debugging.
	 */
	[[nodiscard]] virtual Types::GraphicsApi GetGraphicsApi() const = 0;

	/**
	 * @brief Gets the window's content (DPI) scale
	 * @return std::pair<float, float> The horizontal and vertical scale factors (e.g. {2, 2} on a Retina display)
	 * @details Ratio between the current DPI and the platform's default DPI, as reported by glfwGetWindowContentScale. Use it
	 *			to size backing-pixel content against logical points. Returns {1, 1} if the window is not set.
	 */
	[[nodiscard]] virtual std::pair<float, float> GetContentScale() const = 0;
};

}

#endif //CE_CORE_RENDER_CONTEXT_CONTEXT_HPP
