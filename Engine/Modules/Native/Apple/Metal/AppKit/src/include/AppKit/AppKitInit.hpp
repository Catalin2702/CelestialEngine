//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Metal/AppKit
// File: AppKitInit.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-29
// Updated by: Catalin Chirosca
// Updated: 2026-03-29
//

#pragma once

#ifndef CE_NATIVE_APPLE_METAL_APPKIT_INIT_HPP
#define CE_NATIVE_APPLE_METAL_APPKIT_INIT_HPP

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the CE_AppKit module and ensures Objective-C classes are registered.
 * @details This function must be called before using any CE_AppKit classes (like RenderView)
 *          to ensure the Objective-C runtime has loaded the class definitions.
 *          This is particularly important in unit tests where the dynamic library
 *          might not be automatically loaded.
 * @return true if initialization was successful, false otherwise
 */
bool CE_AppKit_Init(void);

/**
 * @brief Checks if the RenderView class is available in the Objective-C runtime.
 * @return true if RenderView class is registered, false otherwise
 */
bool CE_AppKit_IsRenderViewAvailable(void);

/**
 * @brief Checks if the WindowDelegate class is available in the Objective-C runtime.
 * @return true if WindowDelegate class is registered, false otherwise
 */
bool CE_AppKit_IsWindowDelegateAvailable(void);

#ifdef __cplusplus
}
#endif

#endif //CE_NATIVE_APPLE_METAL_APPKIT_INIT_HPP

