//
// Module: CelestialEngine/Engine/Source/Runtime/Core/Render/Texture
// File: Image.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-23
// Updated by: Catalin Chirosca
// Updated: 2026-11-27
//

#include "Core/Render/Texture/Image.hpp"
#include "IO/FileSystem/FileSystem.hpp"
#include "Tools/Log/Log.hpp"

// The decoder is compiled here and nowhere else: the implementation macro may appear in exactly one translation unit,
// and STB_IMAGE_STATIC gives every stbi_* function internal linkage, so a game that links its own copy of stb_image
// cannot collide with the engine's.
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <limits>
#include <memory>


namespace CE::Core {

namespace {

struct StbiDeleter {
	void operator()(stbi_uc* data) const noexcept { stbi_image_free(data); }
};
using StbiPixels = std::unique_ptr<stbi_uc, StbiDeleter>;

}

std::optional<Image> Image::Load(const std::string_view path) {
	// The bytes come through the file system, never through stbi_load(path): a virtual path is not an OS path, and a
	// mount may one day be an archive with no file on disk at all. The decoder only ever sees memory.
	const auto ioImageBytes = IO::FileSystem::ReadBytes(path);
	if (not ioImageBytes) [[unlikely]] {
		CE_CORE_ERROR("Image::Load: Cannot read '{}' ({})", path, ioImageBytes.error());
		return std::nullopt;
	}

	// stb measures its input with an int.
	if (ioImageBytes->size() > static_cast<size_t>(std::numeric_limits<i32>::max())) [[unlikely]] {
		CE_CORE_ERROR("Image::Load: '{}' is {} bytes, more than the decoder can read!", path, ioImageBytes->size());
		return std::nullopt;
	}

	// Four, whatever the file holds: stb expands grey, grey+alpha and RGB to RGBA, and alpha defaults to opaque.
	constexpr int RequestedChannels = 4;

	i32 width = 0;
	i32 height = 0;
	i32 channelsInFile = 0;

	// No vertical flip. stb hands back the top row first, and on every backend v = 0 reads the first row uploaded -
	// so v = 0 is the top of the image everywhere, and the quad's UVs say so instead of the loader turning it round.
	const StbiPixels pixels{stbi_load_from_memory(
		reinterpret_cast<const stbi_uc*>(ioImageBytes->data()), static_cast<i32>(ioImageBytes->size()),
		&width, &height, &channelsInFile, RequestedChannels
	)};
	if (not pixels) [[unlikely]] {
		CE_CORE_ERROR("Image::Load: Cannot decode '{}' ({})", path, stbi_failure_reason());
		return std::nullopt;
	}

	Image image{
		.width = static_cast<u32>(width),
		.height = static_cast<u32>(height),
		.format = Types::PixelFormat::RGBA8Unorm,
	};

	const auto* imageBytes = reinterpret_cast<const std::byte*>(pixels.get());
	const auto size = static_cast<size_t>(width) * static_cast<size_t>(height) * RequestedChannels;
	image.pixels.assign(imageBytes, imageBytes + size);

	return image;
}

}
