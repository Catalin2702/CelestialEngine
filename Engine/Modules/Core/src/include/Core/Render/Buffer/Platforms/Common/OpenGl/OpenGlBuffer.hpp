//
// Module: CelestialEngine/Engine/Modules/Core/Render/Buffer/Platforms/Common/OpenGl
// File: OpenGlBuffer.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-02
// Updated by: Catalin Chirosca
// Updated: 2026-07-22
//

#pragma once

#ifndef CE_CORE_RENDER_BUFFER_OPENGLBUFFER_HPP
#define CE_CORE_RENDER_BUFFER_OPENGLBUFFER_HPP

#include "Core/Render/Buffer/I_Buffer.hpp"

#include <cstdint>

namespace CE::Core {

/**
 * @class OpenGlVertexBuffer
 * @brief OpenGL implementation of a vertex buffer object (VBO)
 * @details Owns an OpenGL buffer object created on construction and deleted on destruction; Bind/Unbind
 *			attach/detach it to GL_ARRAY_BUFFER.
 */
class OpenGlVertexBuffer final: public I_Buffer {
public:
	/**
	 * @brief Constructor - generates the OpenGL buffer object
	 */
	OpenGlVertexBuffer();
	~OpenGlVertexBuffer() override;

public:
	/**
	 * @brief Binds the buffer to GL_ARRAY_BUFFER
	 */
	void Bind() const override;
	/**
	 * @brief Unbinds any buffer from GL_ARRAY_BUFFER
	 */
	void Unbind() const override;

private:

};

}

#endif //CE_CORE_RENDER_BUFFER_OPENGLBUFFER_HPP
