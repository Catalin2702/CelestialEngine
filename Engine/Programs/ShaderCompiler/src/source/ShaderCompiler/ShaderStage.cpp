//
// Module: CelestialEngine/Engine/Programs/ShaderCompiler
// File: ShaderStage.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-26
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

#include "ShaderCompiler/ShaderStage.hpp"

namespace CE::ShaderCompiler {

std::optional<ShaderStage> StageFromExtension(const std::filesystem::path& path) {
	const auto extension = path.extension();
	if (extension == ".vert") return ShaderStage::Vertex;
	if (extension == ".frag") return ShaderStage::Fragment;

	return std::nullopt;
}

}
