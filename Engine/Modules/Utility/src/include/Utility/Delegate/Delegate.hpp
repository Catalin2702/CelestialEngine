//
// Module: CelestialEngine/Engine/Modules/Utility/Callback
// File: Delegate.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-12
// Updated by: Catalin Chirosca
// Updated: 2026-07-22
//

#pragma once

#ifndef CE_UTILITY_CALLBACK_DELEGATE_HPP
#define CE_UTILITY_CALLBACK_DELEGATE_HPP

#include "Define/DynamicLinker.hpp"

#include <type_traits>

namespace CE::Utility {

template <typename R, typename... Args>
class CE_API Delegate {
public:
	using Stub = R(*)(void*, Args...);

	Delegate() = default;

public:
	// The callables are taken as `auto` non-type parameters constrained on invocability instead of demanding the exact
	// R(Args...) signature: this allows binding targets whose parameters are implicitly convertible from Args (the
	// typical case being a handler taking `const Event&` bound to a dispatcher that delivers `Event&`).

	/**
	 * @brief Creates a delegate bound to a free function
	 * @tparam Fn The function to bind; must be invocable with Args and return a type convertible to R
	 * @return Delegate A delegate that invokes Fn with no bound context
	 */
	template<auto Fn>
		requires std::is_invocable_r_v<R, decltype(Fn), Args...>
	static Delegate FromFunction() {
		Delegate delegate;
		delegate._context = nullptr;
		delegate._stub = &FunctionStub<Fn>;
		return delegate;
	}

	/**
	 * @brief Creates a delegate bound to a member function of the given instance
	 * @tparam T Type of the bound instance
	 * @tparam Method The member function to bind; must be invocable on T& with Args
	 * @param instance The object the method is invoked on (non-owning; must outlive the delegate)
	 * @return Delegate A delegate that invokes Method on the instance
	 */
	template<typename T, auto Method>
		requires std::is_invocable_r_v<R, decltype(Method), T&, Args...>
	static Delegate FromMethod(T* instance) {
		Delegate delegate;
		delegate._context = instance;
		delegate._stub = &MethodStub<T, Method>;
		return delegate;
	}

	/**
	 * @brief Creates a delegate bound to a const member function of the given instance
	 * @tparam T Type of the bound instance
	 * @tparam Method The member function to bind; must be invocable on const T& with Args
	 * @param instance The object the method is invoked on (non-owning; must outlive the delegate)
	 * @return Delegate A delegate that invokes Method on the instance
	 */
	template<typename T, auto Method>
		requires std::is_invocable_r_v<R, decltype(Method), const T&, Args...>
	static Delegate FromConstMethod(const T* instance) {
		Delegate delegate;
		delegate._context = const_cast<T*>(instance);
		delegate._stub = &ConstMethodStub<T, Method>;
		return delegate;
	}

public:
	/**
	 * @brief Checks whether a callable is bound
	 */
	[[nodiscard]] bool IsValid() const { return _stub != nullptr; }

public:
	/**
	 * @brief Invokes the bound callable with the given arguments
	 * @details Undefined behaviour if no callable is bound: check IsValid() first.
	 */
	R operator()(Args... args) const { return (*_stub)(_context, args...); }

	bool operator==(const Delegate& other) const { return this->_context == other._context and this->_stub == other._stub; }

private:
	template<auto Fn>
	static R FunctionStub(void*, Args... args) {
		return Fn(args...);
	}

	template<typename T, auto Method>
	static R MethodStub(void* context, Args... args) {
		return (static_cast<T*>(context)->*Method)(args...);
	}

	template<typename T, auto Method>
	static R ConstMethodStub(void* context, Args... args) {
		return (static_cast<const T*>(context)->*Method)(args...);
	}

private:
	void* _context = nullptr;
	Stub _stub = nullptr;
};

}

#endif //CE_UTILITY_CALLBACK_DELEGATE_HPP

template <typename... Args>
using EventDelegate = CE::Utility::Delegate<void, Args...>;

template <typename R, typename... Args>
using CallbackDelegate = CE::Utility::Delegate<R, Args...>;
