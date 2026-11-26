//
// Module: CelestialEngine/Engine/Source/Runtime/RHI/Surface
// File: I_Surface.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-24
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

#pragma once

#ifndef CE_RHI_SURFACE_I_SURFACE_HPP
#define CE_RHI_SURFACE_I_SURFACE_HPP


namespace CE::RHI {

/**
 * @class I_Surface
 * @brief Something a swapchain can present into, whatever the graphics API
 * @details The RHI's half of the contract with whoever owns the presentable area - a window, today. The RHI cannot
 *			name that owner: windows live in Core, which depends on the RHI, and the dependency must not run the other
 *			way. So the RHI states what it needs as interfaces, one per backend (I_OpenGlSurface, I_MetalSurface), and
 *			the window implements the ones its library can provide. This common base is what I_Swapchain::MakeSwapchain
 *			takes: it asks it for the backend's own surface, and reports a clear error when the window has none.
 *
 *			Deliberately empty but polymorphic: the virtual destructor is what makes the dynamic_cast to the backend
 *			surface possible.
 */
class I_Surface {
public:
	virtual ~I_Surface() = default;
};

}

#endif //CE_RHI_SURFACE_I_SURFACE_HPP
