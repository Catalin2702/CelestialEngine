//
// Module: CelestialEngine/Engine/Modules/Core/Render/Buffer
// File: I_Buffer.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-02
// Updated by: Catalin Chirosca
// Updated: 2026-07-22
//

#pragma once

#ifndef CE_CORE_RENDER_BUFFER_I_BUFFER_HPP
#define CE_CORE_RENDER_BUFFER_I_BUFFER_HPP

namespace CE::Core {

/**
 * @class I_Buffer
 * @brief Interface for GPU buffer objects that can be bound to the render pipeline
 * @details Minimal contract shared by API-specific buffers (vertex, index, ...): make the buffer active
 *			for subsequent draw calls (Bind) or deactivate it (Unbind).
 */
class I_Buffer {
public:
	virtual ~I_Buffer() = default;

public:
	/**
	 * @brief Makes the buffer active for subsequent render commands
	 */
	virtual void Bind() const = 0;
	/**
	 * @brief Deactivates the buffer
	 */
	virtual void Unbind() const = 0;
};

}

#endif //CE_CORE_RENDER_BUFFER_I_BUFFER_HPP
