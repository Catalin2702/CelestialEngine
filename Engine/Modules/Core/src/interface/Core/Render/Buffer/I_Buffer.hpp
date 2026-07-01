//
// Module: CelestialEngine/Engine/Modules/Render/Buffer
// File: I_Buffer.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-02
// Updated by: Catalin Chirosca
// Updated: 2026-07-02
//

#pragma once

#ifndef CE_RENDER_BUFFER_I_BUFFER_HPP
#define CE_RENDER_BUFFER_I_BUFFER_HPP

namespace CE::Render::Buffer {

class I_Buffer {
public:
	virtual ~I_Buffer() = default;

public:
	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;
};

}

#endif //CE_RENDER_BUFFER_I_BUFFER_HPP
