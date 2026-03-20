//
// Module: CelestialEngine/Engine/Modules/Types/Callback
// File: Callback.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-20
// Updated by: Catalin Chirosca
// Updated: 2026-03-20
//

#pragma once

#ifndef CE_TYPES_CALLBACK_CALLBACK_HPP
#define CE_TYPES_CALLBACK_CALLBACK_HPP

#include <functional>


namespace CE::Types {

/**
 * @typedef CallbackFn
 * @brief Type alias for event callback functions
 * @tparam T Event type
 * @details Function that takes an event reference and returns void
 */
template<class T>
using CallbackFn = std::function<void(T&)>;

}

#endif //CE_TYPES_CALLBACK_CALLBACK_HPP
