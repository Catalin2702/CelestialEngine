//
// Module: CelestialEngine/Engine/Source/Runtime/Core/Render/Texture
// File: Image.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-23
// Updated by: Catalin Chirosca
// Updated: 2026-11-27
//

#pragma once

#ifndef CE_CORE_RENDER_TEXTURE_IMAGE_HPP
#define CE_CORE_RENDER_TEXTURE_IMAGE_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Types.hpp"

#include <cstddef>
#include <optional>
#include <string_view>
#include <vector>


namespace CE::Core {

/**
 * @struct Image
 * @brief Decoded texels in CPU memory, ready to become a texture's initial data
 * @details Rows top to bottom, tightly packed, in the format it names - which is what TextureDescriptor::initialData
 *			expects, so an image goes to the GPU with no conversion in between. It is not a texture and knows no
 *			backend: the device copies it once at creation, and the image can be dropped right after.
 */
struct CE_CORE_API Image {
	u32 width = 0;
	u32 height = 0;
	Types::PixelFormat format = Types::PixelFormat::None;

	std::vector<std::byte> pixels{};

	/**
	 * @brief Decodes a PNG, JPEG, TGA or BMP file into RGBA8 texels
	 * @param path Virtual path of the file, e.g. "resources://Textures/Logo.png"
	 * @return std::optional<Image> The decoded image, or nothing when the file is missing or is not an image
	 * @details Always four channels, whatever the file stores: the engine has no three-channel format (see
	 *			PixelFormat), and one format for every image keeps the upload path free of conversions. A failure is
	 *			logged here, with the decoder's own reason, and is not fatal: the caller decides what to draw instead.
	 */
	[[nodiscard]] static std::optional<Image> Load(std::string_view path);
};

}

#endif // CE_CORE_RENDER_TEXTURE_IMAGE_HPP
