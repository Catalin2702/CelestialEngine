//
// Module: CelestialEngine/Engine/Modules/Core/Render/Buffer/Platforms/Common/OpenGl
// File: OpenGlBuffer.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-02
// Updated by: Catalin Chirosca
// Updated: 2026-07-13
//

#pragma once

#ifndef CE_CORE_RENDER_BUFFER_OPENGLBUFFER_HPP
#define CE_CORE_RENDER_BUFFER_OPENGLBUFFER_HPP

#include "Core/Render/Buffer/I_Buffer.hpp"

#include <cstdint>

namespace CE::Core {

class OpenGlVertexBuffer final: public I_Buffer {
public:
	OpenGlVertexBuffer();
	~OpenGlVertexBuffer() override;

public:
	void Bind() const override;
	void Unbind() const override;

private:

};

}

#endif //CE_CORE_RENDER_BUFFER_OPENGLBUFFER_HPP
