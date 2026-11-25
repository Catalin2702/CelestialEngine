//
// Module: CelestialEngine/Engine/Modules/Core/Config
// File: Config.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-26
// Updated by: Catalin Chirosca
// Updated: 2026-11-25
//

#pragma once

#ifndef CE_CORE_CONFIG_CONFIG_HPP
#define CE_CORE_CONFIG_CONFIG_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Window/WindowProps.hpp"

#include <concepts>
#include <functional>
#include <memory>
#include <utility>


namespace CE::Core {

/**
 * @class Config
 * @brief Global engine configuration store, safe to read and write from any thread
 * @details Holds the runtime configuration - today the window properties parsed from the command line - behind static
 *			methods only: there is no instance to get, and the class cannot be constructed.
 *
 *			The data and its std::shared_mutex live in Config.cpp, in a function-local static (built on first use, with
 *			the thread-safe initialisation C++ guarantees for those), so this header - which reaches every client
 *			through Core.hpp - pulls in no <shared_mutex>. Reads take a shared lock and can run side by side; writes take
 *			an exclusive one. Every access is one short critical section: a copy in or out of a small struct.
 *
 *			Nothing is handed out by reference: a reference would outlive the lock that protected it, and a writer on
 *			another thread could then reallocate the title under the reader's feet. GetWindowProps returns a copy -
 *			cheap, since WindowProps is a few integers and a title short enough for the string's small-buffer storage -
 *			and a change made from what was read goes through UpdateWindowProps, which reads and writes under one lock.
 */
class CE_CORE_API Config {
public:
	Config() = delete;
	Config(const Config&) = delete;
	Config(Config&&) noexcept = delete;
	~Config() = delete;

public:
	Config& operator = (const Config&) = delete;
	Config& operator = (Config&&) noexcept = delete;

public:
	/**
	 * @brief Gets a copy of the current window properties
	 * @return Types::WindowProps The stored window configuration (size, APIs, VSync, ...); default-constructed values
	 *		   (empty title, zero size, no API) until something is stored
	 * @details Returned by value on purpose: see the class description. Bind it to a local
	 *			(`const auto windowProps = Config::GetWindowProps();`) and read the fields from there, rather than calling
	 *			it once per field - each call takes the lock again and may see a different write.
	 */
	[[nodiscard]] static Types::WindowProps GetWindowProps();

public:
	/**
	 * @brief Replaces the stored window properties
	 * @param newWindowProps The new window configuration, moved in under the lock
	 */
	static void SetWindowProps(Types::WindowProps newWindowProps);

	/**
	 * @brief Changes the stored window properties in place, reading and writing them under one exclusive lock
	 * @param function Called once with the stored properties; whatever it changes is the new configuration
	 * @details The way to change one field from its current value (e.g. toggling VSync): a GetWindowProps followed by
	 *			a SetWindowProps takes two locks, and a write from another thread between them would be lost.
	 *			`function` runs with the lock held, so it must be short and must not call Config itself - the lock is not
	 *			recursive, and that call would deadlock. No allocation and no copy of the callable: Config.cpp receives
	 *			a pointer to it plus a plain function that knows its type.
	 * @example
	 *			bool vSync = false;
	 *			Config::UpdateWindowProps([&vSync](Types::WindowProps& props) { vSync = props.VSync = not props.VSync; });
	 */
	template <typename Function>
	requires std::invocable<Function, Types::WindowProps&>
	static void UpdateWindowProps(Function&& function) {
		// Forwarded into a reference of the same value category, and invoked as such below: an rvalue callable is
		// called as an rvalue, so an operator() && overload is honoured.
		auto&& forwarded = std::forward<Function>(function);
		// What crosses into Config.cpp is the address of this pointer, not of the callable: the pointer keeps the
		// callable's exact type - const included - and is itself a non-const object, so a static_cast to void* is
		// enough and nothing has its constness removed.
		auto* callable = std::addressof(forwarded);

		_UpdateWindowProps(
			[](void* erased, Types::WindowProps& windowProps) {
				std::invoke(std::forward<Function>(**static_cast<decltype(callable)*>(erased)), windowProps);
			},
			static_cast<void*>(&callable));
	}

private:
	/// The non-template half of UpdateWindowProps: takes the lock and calls `invoke(callable, props)`.
	static void _UpdateWindowProps(void (*invoke)(void*, Types::WindowProps&), void* callable);
};

}

#endif //CE_CORE_CONFIG_CONFIG_HPP
