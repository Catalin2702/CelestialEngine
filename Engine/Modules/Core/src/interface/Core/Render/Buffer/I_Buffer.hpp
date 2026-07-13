//
// Module: CelestialEngine/Engine/Modules/Core/Render/Buffer
// File: I_Buffer.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-02
// Updated by: Catalin Chirosca
// Updated: 2026-07-13
//

#pragma once

#ifndef CE_CORE_RENDER_BUFFER_I_BUFFER_HPP
#define CE_CORE_RENDER_BUFFER_I_BUFFER_HPP

namespace CE::Core {

class I_Buffer {
public:
	virtual ~I_Buffer() = default;

public:
	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;
};

}

#endif //CE_CORE_RENDER_BUFFER_I_BUFFER_HPP
