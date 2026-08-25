//
// Module: CelestialEngine/Engine/Modules/Utility/Range
// File: Enumerate.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-25
// Updated by: Catalin Chirosca
// Updated: 2026-08-25
//

#pragma once

#ifndef CE_UTILITY_RANGE_ENUMERATE_HPP
#define CE_UTILITY_RANGE_ENUMERATE_HPP

#include <tuple>

namespace CE::Utility {

template<typename T, typename Iter = decltype(std::begin(std::declval<T>())), typename = decltype(std::end(std::declval<T>()))>
constexpr auto Enumerate(T&& iterable) {
	struct Iterator {
		size_t index;
		Iter iter;

		bool operator != (const Iterator& other) const { return iter != other.iter; }
		void operator ++ () { ++index; ++iter; }
		auto operator * () const { return std::tie(index, *iter); }
	};

	struct IterableWrapper {
		T iterable;

		auto begin() { return Iterator{.index = 0, .iter = std::begin(iterable)}; }
		auto end() { return Iterator{.index = 0, .iter = std::end(iterable)}; }
	};
	return IterableWrapper{ std::forward<T>(iterable)};
}

}

#endif //CE_UTILITY_RANGE_ENUMERATE_HPP
