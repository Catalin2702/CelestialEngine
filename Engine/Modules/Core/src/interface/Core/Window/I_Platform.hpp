//
// Module: CelestialEngine/Engine/Modules/Core/Window
// File: I_Platform.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-02
// Updated by: Catalin Chirosca
// Updated: 2026-09-02
//

#pragma once

#ifndef CE_CORE_WINDOW_I_PLATFORM_HPP
#define CE_CORE_WINDOW_I_PLATFORM_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Window/WindowProps.hpp"
#include "Utility/Delegate/Dispatcher.hpp"

#include <memory>


namespace CE::Core {

class I_EventHubDispatcher;

/**
 * @class I_Platform
 * @brief The windowing library's own lifetime and its process-wide event queue
 * @details Everything a windowing backend owns once per process rather than once per window: bringing the library up
 *			and tearing it down, and draining the queue that feeds every window's callbacks. Keeping it out of I_Window
 *			is not tidiness - none of these is per-window on any backend, and a window that owned them would have to
 *			reference-count itself to know when the last one is going away.
 *
 *			Constructed before any window and destroyed after all of them, which the owner gets for free by declaring
 *			it first: members are destroyed in reverse declaration order.
 */
class CE_CORE_API I_Platform {
public:
	virtual ~I_Platform() = default;

public:
	/**
	 * @brief Brings up the windowing library selected by @p windowApi
	 * @param windowApi Which backend to initialise
	 * @return std::unique_ptr<I_Platform> The live library, which must outlive every window built from it
	 * @details Throws rather than returning null: an application that cannot open a window has nothing left to do.
	 */
	[[nodiscard]] static std::unique_ptr<I_Platform> MakePlatform(Types::WindowApi windowApi);

public:
	/**
	 * @brief Drains the event queue and delivers the callbacks
	 * @details One queue for the whole process however many windows exist, which is why this is called once per frame
	 *			by the run loop and never by a window. A backend whose events are delivered by the OS run loop itself,
	 *			as on Cocoa, has nothing to do here.
	 */
	virtual void PollEvents() const = 0;

	/**
	 * @brief Routes this backend's raw callbacks into the event hub
	 * @details The translation from a native callback to an engine event is backend-specific - GLFW hands out ints,
	 *			AppKit hands out NS::Event* - so the wiring lives here, where the backend is already known, instead of
	 *			in the application, which would otherwise have to downcast to do it.
	 */
	virtual void ConnectToEventHub(I_EventHubDispatcher& eventHub) = 0;

public:
	/**
	 * @brief Gets which windowing backend this is
	 * @details Lets a factory check what it is holding before downcasting to reach a backend-specific facility.
	 */
	[[nodiscard]] virtual Types::WindowApi GetWindowApi() const = 0;

public:
	/// @brief Fires for every error the windowing library reports, including failures during its own initialisation.
	/// @details A plain member rather than a virtual getter because it is the same channel on every backend, and one
	///			 a backend with nothing to report simply never fires.
	UnicastDispatcher<int, const char*> onErrorDispatcher;
};

template<Types::WindowApi Api>
class I_PlatformBase: public I_Platform {
public:
	[[nodiscard]] Types::WindowApi GetWindowApi() const override { return _api; }

private:
	static constexpr Types::WindowApi _api = Api;
};

}

#endif //CE_CORE_WINDOW_I_PLATFORM_HPP
