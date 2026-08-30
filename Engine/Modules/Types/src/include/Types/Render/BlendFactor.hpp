//
// Module: CelestialEngine/Engine/Modules/Types/Render
// File: BlendFactor.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-29
// Updated by: Catalin Chirosca
// Updated: 2026-08-30
//

#pragma once

#ifndef CE_TYPES_RENDER_BLENDFACTOR_HPP
#define CE_TYPES_RENDER_BLENDFACTOR_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Var/Vars.hpp"


namespace CE::Types {

/**
 * @enum BlendFactor
 * @brief Coefficient a colour is multiplied by before the two sides of a blend are combined
 * @details The blend equation is `result = (source * srcFactor) op (destination * dstFactor)`, and a BlendFactor
 *			names one of those two coefficients. Deliberately NOT a set of flags: the factors are mutually
 *			exclusive choices, so combining them with `|` would produce a value no API can honour.
 *
 *			The listed set is the intersection of OpenGL, DirectX 11/12, Vulkan and Metal - every entry maps to a
 *			real constant in all five. The blend colour the Constant* factors refer to is set per render pass, not
 *			per pipeline, in every API that supports it.
 */
enum class BlendFactor: u8 {
	Zero,						///< 0 - the term is dropped entirely
	One,						///< 1 - the term is taken as-is

	SrcColor,					///< the incoming fragment's RGB
	OneMinusSrcColor,			///< 1 - the incoming fragment's RGB
	DstColor,					///< the RGB already in the render target
	OneMinusDstColor,			///< 1 - the RGB already in the render target

	SrcAlpha,					///< the incoming fragment's alpha - the source side of ordinary alpha blending
	OneMinusSrcAlpha,			///< 1 - the incoming fragment's alpha - the destination side of the same
	DstAlpha,					///< the alpha already in the render target
	OneMinusDstAlpha,			///< 1 - the alpha already in the render target

	ConstantColor,				///< the RGB of the blend constant set on the encoder
	OneMinusConstantColor,		///< 1 - the RGB of the blend constant
	ConstantAlpha,				///< the alpha of the blend constant
	OneMinusConstantAlpha,		///< 1 - the alpha of the blend constant

	/// min(srcAlpha, 1 - dstAlpha), replicated across RGB and forced to 1 for alpha. Used by order-independent
	/// transparency schemes; valid as a source factor only, every API rejects it on the destination side.
	SrcAlphaSaturated,
};

/**
 * @brief Converts a BlendFactor enum value to its string representation
 * @param blendFactor The BlendFactor to convert to a string
 * @return const char* A string representation of the BlendFactor, or "Unknown" if the value is out of range
 */
CE_TYPES_API const char* ToString(BlendFactor blendFactor);

/**
 * @brief Checks whether a blend factor is legal on the destination side of the blend equation
 * @param blendFactor The BlendFactor to check
 * @return bool False only for SrcAlphaSaturated, true otherwise
 * @details Worth asserting on when a BlendState is built, because the failure is otherwise reported by the
 *			driver at pipeline creation - and only on the backends that bother to validate it.
 */
CE_TYPES_API bool IsValidDestinationFactor(BlendFactor blendFactor);

CE_TYPES_API u32 ToOpenGl(BlendFactor blendFactor);

}

#endif //CE_TYPES_RENDER_BLENDFACTOR_HPP
