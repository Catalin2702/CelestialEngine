//
// Module: CelestialEngine/Engine/Modules/Core/Render/Renderer
// File: RenderStats.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-02
// Updated by: Catalin Chirosca
// Updated: 2026-09-02
//

#pragma once

#ifndef CE_CORE_RENDER_RENDERER_RENDERSTATS_HPP
#define CE_CORE_RENDER_RENDERER_RENDERSTATS_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Var/Vars.hpp"


namespace CE::Core {

/**
 * @struct RenderStats
 * @brief What one frame cost, counted on the CPU side
 * @details Counted rather than measured: these are submissions the renderer made, not work the GPU finished, and
 *			nothing here needs a query or a fence. GPU timings are a separate concern, because reading them back
 *			always lags a frame or more behind.
 */
struct CE_CORE_API RenderStats {
	///< Commands that reached the encoder; the ones skipped as invalid are not counted
	u32 drawCalls = 0;

	///< Render passes opened this frame
	u32 passes = 0;

	///< Indices asked for across every draw - the closest thing to a triangle count that holds for every topology
	u64 indices = 0;

	void Reset() { *this = RenderStats{}; }
};

}

#endif //CE_CORE_RENDER_RENDERER_RENDERSTATS_HPP
