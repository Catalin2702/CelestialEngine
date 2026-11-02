//
// Module: CelestialEngine/Engine/Modules/Types/Render
// File: PrimitiveTopology.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-29
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#include "Types/Render/PrimitiveTopology.hpp"


namespace CE::Types {

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

}
