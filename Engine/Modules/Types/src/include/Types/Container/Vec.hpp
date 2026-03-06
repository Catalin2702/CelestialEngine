//
// Module: CelestialEngine/Engine/Modules/Types
// File: Vec.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-06
// Updated by: Catalin Chirosca
// Updated: 2026-03-06
//

#pragma once

#ifndef CE_TYPES_CONTAINER_VEC_HPP
#define CE_TYPES_CONTAINER_VEC_HPP

namespace CE::Types {

/**
 * @struct Vec2
 * @brief 2D vector template struct
 * @tparam T Numeric type of the vector components (e.g., int, float, double)
 * @details Simple 2D vector struct with x and y components. Provides constructors for default initialization (0, 0) and parameterized initialization.
 */
template<typename T>
struct Vec2 {
	T x, y;

	Vec2() : x(0), y(0) {}
	Vec2(T x, T y) : x(x), y(y) {}
};

using Vec2i = Vec2<int>;
using Vec2f = Vec2<float>;
using Vec2d = Vec2<double>;

}

#endif //CE_TYPES_CONTAINER_VEC_HPP
