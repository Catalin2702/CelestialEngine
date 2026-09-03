//
// Module: CelestialEngine/Engine/Modules/Types/Render
// File: AttachmentAction.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-31
// Updated by: Catalin Chirosca
// Updated: 2026-09-03
//

#pragma once

#ifndef CE_TYPES_RENDER_ATTACHMENTACTION_HPP
#define CE_TYPES_RENDER_ATTACHMENTACTION_HPP

#include "Types/Var/Vars.hpp"

#include <format>
#include <string_view>


namespace CE::Types {

/**
 * @enum LoadAction
 * @brief What happens to an attachment's existing contents when a render pass begins
 * @details On a tile-based GPU - which is every Apple Silicon one - the choice is real bandwidth: Load copies the
 *			attachment from main memory into tile memory before the pass runs, the other two do not.
 */
enum class LoadAction: u8 {
	Load,		///< Keep what is already there - the only choice when the pass draws on top of an earlier one
	Clear,		///< Fill with the attachment's clear value
	DontCare,	///< Contents are undefined; promise that every pixel read is one this pass wrote first
};

/**
 * @enum StoreAction
 * @brief What happens to an attachment's contents when a render pass ends
 * @details The mirror image of LoadAction: DontCare lets a tile-based GPU skip writing the tile back to memory
 *			entirely, which is what makes a depth buffer nobody reads afterwards nearly free.
 */
enum class StoreAction: u8 {
	Store,		///< Write the result back, so a later pass can sample or load it
	DontCare,	///< Discard it; the attachment was scaffolding for this pass only
};


/**
 * @brief Names a LoadAction, for fmt/spdlog and - through the formatter below - for std::format
 * @param loadAction The value to name
 * @return std::string_view The enumerator's name, or "Unknown" for a value outside the enum
 */
constexpr std::string_view format_as(const LoadAction loadAction) {
	switch (loadAction) {
		case LoadAction::Load: return "Load";
		case LoadAction::Clear: return "Clear";
		case LoadAction::DontCare: return "DontCare";
		default: return "Unknown";
	}
}

/**
 * @brief Names a StoreAction, for fmt/spdlog and - through the formatter below - for std::format
 * @param storeAction The value to name
 * @return std::string_view The enumerator's name, or "Unknown" for a value outside the enum
 */
constexpr std::string_view format_as(const StoreAction storeAction) {
	switch (storeAction) {
		case StoreAction::Store: return "Store";
		case StoreAction::DontCare: return "DontCare";
		default: return "Unknown";
	}
}

}

template <>
struct std::formatter<CE::Types::LoadAction>: std::formatter<std::string_view> {
	auto format(const CE::Types::LoadAction value, std::format_context& ctx) const {
		return std::formatter<std::string_view>::format(format_as(value), ctx);
	}
};

template <>
struct std::formatter<CE::Types::StoreAction>: std::formatter<std::string_view> {
	auto format(const CE::Types::StoreAction value, std::format_context& ctx) const {
		return std::formatter<std::string_view>::format(format_as(value), ctx);
	}
};

#endif //CE_TYPES_RENDER_ATTACHMENTACTION_HPP
