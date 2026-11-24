//
// Module: CelestialEngine/Engine/Modules/RHI/Buffer
// File: Buffer.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-25
// Updated by: Catalin Chirosca
// Updated: 2026-11-24
//

#include "RHI/Buffer/I_Buffer.hpp"

namespace CE::RHI {

BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements): _elements(elements) {
	_CalculateOffsetsAndStride();
}

void BufferLayout::_CalculateOffsetsAndStride() {
	for (auto& element: _elements) {
		element.offset = _stride;
		_stride += element.size;
	}
}

}
