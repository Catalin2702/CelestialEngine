//
// Module: CelestialEngine/Engine/Modules/Types/Render
// File: PrimitiveTopology.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-29
// Updated by: Catalin Chirosca
// Updated: 2026-08-30
//

#include "Types/Render/PrimitiveTopology.hpp"

#include <glad/glad.h>

namespace CE::Types {

const char* ToString(const PrimitiveTopology topology) {
	switch (topology) {
		case PrimitiveTopology::PointList: return "PointList";
		case PrimitiveTopology::LineList: return "LineList";
		case PrimitiveTopology::LineStrip: return "LineStrip";
		case PrimitiveTopology::TriangleList: return "TriangleList";
		case PrimitiveTopology::TriangleStrip: return "TriangleStrip";
		default: return "Unknown";
	}
}

u32 GetPrimitiveCount(const PrimitiveTopology topology, const u32 vertexCount) {
	switch (topology) {
		case PrimitiveTopology::PointList:
			return vertexCount;

		case PrimitiveTopology::LineList:
			return vertexCount / 2;

		case PrimitiveTopology::LineStrip:
			return vertexCount < 2 ? 0 : vertexCount - 1;

		case PrimitiveTopology::TriangleList:
			return vertexCount / 3;

		case PrimitiveTopology::TriangleStrip:
			return vertexCount < 3 ? 0 : vertexCount - 2;

		default:
			return 0;
	}
}

u32 ToOpenGl(const PrimitiveTopology topology) {
	switch (topology) {
		case PrimitiveTopology::PointList: return GL_POINTS;
		case PrimitiveTopology::LineList: return GL_LINES;
		case PrimitiveTopology::LineStrip: return GL_LINE_STRIP;
		case PrimitiveTopology::TriangleStrip: return GL_TRIANGLE_STRIP;
		case PrimitiveTopology::TriangleList:
		default:
			return GL_TRIANGLES;
	}
}

}
