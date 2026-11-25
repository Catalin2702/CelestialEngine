//
// Module: CelestialEngine/Engine/Modules/Core/Config
// File: Config.cpp
// Created by: Catalin Chirosca
// Created: 2026-05-26
// Updated by: Catalin Chirosca
// Updated: 2026-11-25
//

#include "Core/Config/Config.hpp"

#include <mutex>
#include <shared_mutex>


namespace CE::Core {

namespace {

/**
 * @brief The stored configuration and the lock that guards it
 * @details A shared_mutex rather than a plain mutex: reads (every window, layer and the application, possibly from
 *			several threads once there are any) far outnumber writes (the command line once, the VSync menu item), and
 *			readers then never wait for each other.
 */
struct ConfigStore {
	std::shared_mutex mutex;
	Types::WindowProps windowProps;
};

ConfigStore& Store() {
	// Built on first use, not at load time: nothing guarantees that a static initialiser elsewhere does not read the
	// configuration before this translation unit's statics exist. The initialisation itself is thread-safe (C++11), and
	// the store lives in static storage - no heap allocation, no pointer to check on every access.
	static ConfigStore store;
	return store;
}

}

Types::WindowProps Config::GetWindowProps() {
	auto& [mutex, windowProps] = Store();
	const std::shared_lock lock{mutex};
	return windowProps;
}

void Config::SetWindowProps(Types::WindowProps windowProps) {
	auto& [mutex, _windowProps] = Store();
	const std::unique_lock lock{mutex};
	// Moved, not copied: the only work done under the lock is swapping the title's storage.
	_windowProps = std::move(windowProps);
}

void Config::_UpdateWindowProps(void (*invoke)(void*, Types::WindowProps&), void* callable) {
	auto& [mutex, windowProps] = Store();
	const std::unique_lock lock{mutex};
	invoke(callable, windowProps);
}

}
