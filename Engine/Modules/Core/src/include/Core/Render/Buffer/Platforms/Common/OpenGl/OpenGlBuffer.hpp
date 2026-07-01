//
// Module: CelestialEngine/Engine/Modules/Render/Buffer
// File: OpenGlBuffer.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-02
// Updated by: Catalin Chirosca
// Updated: 2026-07-02
//

#pragma once

#ifndef CE_RENDER_BUFFER_OPENGLBUFFER_HPP
#define CE_RENDER_BUFFER_OPENGLBUFFER_HPP

#include "Core/Render/Buffer/I_Buffer.hpp"

#include <cstdint>

namespace CE::Render::Buffer {

class OpenGlVertexBuffer final: public I_Buffer {
public:
	OpenGlVertexBuffer();
	~OpenGlVertexBuffer() override;

public:
	void Bind() const override;
	void Unbind() const override;

private:
	uint32_t
};

}

#endif //CE_RENDER_BUFFER_OPENGLBUFFER_HPP
