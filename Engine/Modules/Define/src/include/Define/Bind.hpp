//
// Module: CelestialEngine/Engine/Modules/Define
// File: Bind.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-28
// Updated by: Catalin Chirosca
// Updated: 2026-03-21
//

#pragma once

#ifndef CE_DEFINE_BIND_HPP
#define CE_DEFINE_BIND_HPP

/**
 * @brief Macro to bind a member function with no parameters to a lambda
 * @param fn Member function name to bind
 * @details Creates a lambda that captures 'this' and calls the specified member function
 *			without any parameters. Useful for creating callbacks from member functions.
 */
#define BIND_FN_NO_PARAMS(fn) [this]() { return fn(); }

/**
 * @brief Macro to bind a member function with one parameter to a lambda
 * @param fn Member function name to bind
 * @details Creates a lambda that captures 'this' and forwards a single argument to the
 *			specified member function. Uses perfect forwarding to preserve argument types.
 *			Useful for creating event callbacks from member functions.
 */
#define BIND_FN_ONE_PARAM(fn) [this](auto&& arg) { return fn(std::forward<decltype(arg)>(arg)); }

/**
 * @brief Macro to bind a member function with any number of parameters to a lambda
 * @param fn Member function name to bind
 * @details Creates a lambda that captures 'this' and forwards any number of arguments to the
 *			specified member function. Uses perfect forwarding to preserve argument types.
 *			Useful for creating event callbacks from member functions that take multiple parameters.
 */
#define BIND_FN_ANY_PARAMS(fn) [this](auto&&... args) { return fn(std::forward<decltype(args)>(args)...); }

/**
 * @brief Macro to bind a member function with one parameter to a lambda on a specific instance
 * @param instance Instance pointer to bind the member function on
 * @param fn Member function name to bind
 * @details Creates a lambda that captures the specified instance and calls the member function
 *			on that instance without any parameters. Useful for creating callbacks from member functions
 *			on objects other than 'this'.
 */
#define BIND_FN_ONE_PARAM_ON(instance, fn) [inst = (instance)](auto&& arg) { return (inst->*fn)(std::forward<decltype(arg)>(arg)); }

/**
 * @brief Macro to bind a member function with any number of parameters to a lambda on a specific instance
 * @param instance Instance pointer to bind the member function on
 * @param fn Member function name to bind
 * @details Creates a lambda that captures the specified instance and forwards any number of arguments
 *			to the member function on that instance. Uses perfect forwarding to preserve argument types.
 *			Useful for creating event callbacks from member functions on objects other than 'this' that take multiple parameters.
 */
#define BIND_FN_ANY_PARAMS_ON(instance, fn) [inst = (instance)](auto&&... args) { return (inst->*fn)(std::forward<decltype(args)>(args)...); }

#endif //CE_DEFINE_BIND_HPP
