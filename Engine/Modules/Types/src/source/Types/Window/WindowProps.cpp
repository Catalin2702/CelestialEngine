//
// Module: Types/Window
// File: WindowProps.cpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-21
// Updated: 2026-02-28
//

#include "Types/Window/WindowProps.hpp"


namespace CE::Types::Window {

/**
 * @brief Constructor
 * @param title Window title string
 * @param width Window width in pixels
 * @param height Window height in pixels
 * @param VSync Enable or disable vertical synchronization
 * @param graphicsApi Graphics API to use for rendering
 * @details Initializes the WindowProps structure with the provided values.
 *			This structure is used to pass configuration parameters when creating a window.
 */
WindowProps::WindowProps(const std::string& title, const unsigned int width, const unsigned int height, const bool VSync, const GraphicsApi graphicsApi): // NOLINT(*-pass-by-value)
	title(title), width(width), height(height), VSync(VSync), graphicsApi(graphicsApi) {}

}
