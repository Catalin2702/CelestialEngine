//
// Module: CelestialEngine/Engine/Modules/Core/Window/Platforms/Common/Glfw
// File: GlfwPlatform.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-02
// Updated by: Catalin Chirosca
// Updated: 2026-09-02
//

#pragma once

#ifndef CE_CORE_WINDOW_COMMON_GLFWPLATFORM_HPP
#define CE_CORE_WINDOW_COMMON_GLFWPLATFORM_HPP

#include "Core/Window/I_Platform.hpp"

#include "Define/DynamicLinker.hpp"
#include "Types/Types.hpp"


namespace CE::Core {

/**
 * @class GlfwPlatform
 * @brief The GLFW library's lifetime, owned once instead of counted per window
 * @details glfwInit, glfwTerminate, the error callback, the event queue and the window hints are all process state,
 *			not window state. Holding them here as an RAII object removes the reference count a per-window owner
 *			needed, makes double initialisation impossible, and lets the error callback reach a dispatcher that
 *			exists before the first window does - which is exactly the window GLFW reports creation failures in.
 *
 *			Owned by the application and declared before the window, so the library is up before a window needs it and
 *			torn down after the last one is gone.
 */
class CE_CORE_API GlfwPlatform final: public I_PlatformBase<Types::WindowApi::GLFW> {
public:
	/**
	 * @brief Registers the error callback, then initialises GLFW
	 * @details In that order on purpose: GLFW reports initialisation failures through the error callback, so
	 *			registering it afterwards is the difference between a diagnosable failure and a silent one.
	 */
	GlfwPlatform();

	GlfwPlatform(const GlfwPlatform&) = delete;

	GlfwPlatform(GlfwPlatform&&) = delete;

	/**
	 * @brief Terminates GLFW, destroying any window still open
	 */
	~GlfwPlatform() override;

public:
	GlfwPlatform& operator = (const GlfwPlatform&) = delete;
	GlfwPlatform& operator = (GlfwPlatform&&) = delete;

public:
	/**
	 * @brief Drains the event queue and delivers the callbacks
	 * @details One queue for the whole process, whatever the number of windows, which is why this is called once per
	 *			frame by the run loop and never by a window.
	 */
	void PollEvents() const override;

	/**
	 * @brief Applies the window hints the next window creation will use
	 * @details Which hints is a graphics decision, not a windowing one: OpenGL wants the 4.1 core profile macOS caps
	 *			out at, while Metal and Vulkan bring their own surface and need GLFW to create no context at all.
	 *			Resets to the defaults first, so hints never leak from one window to the next.
	 */
	void ApplyWindowHints(Types::GraphicsApi graphicsApi) const;
};

}

#endif //CE_CORE_WINDOW_COMMON_GLFWPLATFORM_HPP
