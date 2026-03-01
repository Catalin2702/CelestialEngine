//
// Module: Define
// File: Bind.hpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-28
// Updated: 2026-02-28
//

#pragma once

#ifndef CE_DEFINE_BIND_HPP
#define CE_DEFINE_BIND_HPP

/**
 * @brief Macro to bind a member function with no parameters to a lambda
 * @param fn Member function name to bind
 * @details Creates a lambda that captures 'this' and calls the specified member function
 *          without any parameters. Useful for creating callbacks from member functions.
 */
#define BIND_FN_NO_PARAMS(fn) [this]() { return fn(); }

/**
 * @brief Macro to bind a member function with one parameter to a lambda
 * @param fn Member function name to bind
 * @details Creates a lambda that captures 'this' and forwards a single argument to the
 *          specified member function. Uses perfect forwarding to preserve argument types.
 *          Useful for creating event callbacks from member functions.
 */
#define BIND_FN_ONE_PARAM(fn) [this](auto&& arg) { return fn(std::forward<decltype(arg)>(arg)); }

#endif //CE_DEFINE_BIND_HPP
