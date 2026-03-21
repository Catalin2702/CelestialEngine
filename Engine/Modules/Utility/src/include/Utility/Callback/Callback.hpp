//
// Module: CelestialEngine/Engine/Modules/Utility/Callback
// File: Callback.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-21
// Updated by: Catalin Chirosca
// Updated: 2026-03-21
//

#pragma once

#ifndef CE_UTILITY_CALLBACK_CALLBACK_HPP
#define CE_UTILITY_CALLBACK_CALLBACK_HPP

#include <functional>

namespace CE::Utility {

/**
 * @typedef CallbackFn
 * @brief Type alias for event callback functions
 * @tparam T The type of event that the callback will handle
 * @details Function that takes an event reference and returns void
 */
template<class T>
using CallbackFn = std::function<void(T&)>;

}

#endif //CE_UTILITY_CALLBACK_CALLBACK_HPP
