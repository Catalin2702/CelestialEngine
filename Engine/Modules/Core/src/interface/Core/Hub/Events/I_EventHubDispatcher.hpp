//
// Module: CelestialEngine/Engine/Modules/Core/Hub/Events
// File: I_EventHubDispatcher.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-02
// Updated by: Catalin Chirosca
// Updated: 2026-09-02
//

#pragma once

#ifndef CE_CORE_HUB_EVENTS_I_EVENTHUBDISPATCHER_HPP
#define CE_CORE_HUB_EVENTS_I_EVENTHUBDISPATCHER_HPP

#include "Core/Hub/Events/I_ApplicationEventHubDispatcher.hpp"
#include "Core/Hub/Events/I_KeyboardEventHubDispatcher.hpp"
#include "Core/Hub/Events/I_MouseEventHubDispatcher.hpp"
#include "Core/Hub/Events/I_RenderContextEventHubDispatcher.hpp"
#include "Core/Hub/Events/I_WindowEventHubDispatcher.hpp"

#include "Define/DynamicLinker.hpp"
#include "Types/Window/WindowProps.hpp"
#include "Utility/Delegate/Dispatcher.hpp"


namespace CE::Core {

/**
 * @struct ApplicationEventHub
 * @brief The multicast channels every application lifecycle event is delivered on
 * @details Defined once here rather than per backend: the channels are the engine's own events, so they do not vary
 *			with the windowing library that happens to produce them. It is only the raw callbacks feeding the hub that
 *			are platform-specific, and those stop at the Receive* methods.
 */
struct ApplicationEventHub {
	MulticastDispatcher<Events::ErrorEvent&> onErrorMulticastDispatcher;
	MulticastDispatcher<Events::AppRenderEvent&> onRenderMulticastDispatcher;
	MulticastDispatcher<Events::AppTickEvent&> onTickMulticastDispatcher;
	MulticastDispatcher<Events::AppUpdateEvent&> onUpdateMulticastDispatcher;
};

/**
 * @struct KeyboardEventHub
 * @brief The multicast channels every keyboard event is delivered on
 */
struct KeyboardEventHub {
	MulticastDispatcher<Events::KeyPressedEvent&> onPressedMulticastDispatcher;
	MulticastDispatcher<Events::KeyReleasedEvent&> onReleasedMulticastDispatcher;
	MulticastDispatcher<Events::KeyTypedEvent&> onTypedMulticastDispatcher;
};

/**
 * @struct MouseEventHub
 * @brief The multicast channels every mouse event is delivered on
 */
struct MouseEventHub {
	MulticastDispatcher<Events::MouseMovedEvent&> onMovedMulticastDispatcher;
	MulticastDispatcher<Events::MouseButtonPressedEvent&> onButtonPressedMulticastDispatcher;
	MulticastDispatcher<Events::MouseButtonReleasedEvent&> onButtonReleasedMulticastDispatcher;
	MulticastDispatcher<Events::MouseDraggedEvent&> onDraggedMulticastDispatcher;
	MulticastDispatcher<Events::MouseWheelScrolledEvent&> onWheelScrolledMulticastDispatcher;
};

/**
 * @struct WindowEventHub
 * @brief The multicast channels every window event is delivered on
 */
struct WindowEventHub {
	MulticastDispatcher<Events::WindowCloseEvent&> onCloseMulticastDispatcher;
	MulticastDispatcher<Events::ErrorEvent&> onErrorMulticastDispatcher;
	MulticastDispatcher<Events::WindowResizeEvent&> onResizeMulticastDispatcher;
	MulticastDispatcher<Events::WindowFocusEvent&> onFocusMulticastDispatcher;	///< Input state resets held keys on focus loss
};

/**
 * @struct RenderContextEventHub
 * @brief The multicast channels every render context event is delivered on
 * @details The one hub a backend may legitimately extend, because render backends genuinely differ in what they can
 *			report - the Metal path adds a drawable resize its view raises and OpenGL has no equivalent of. A backend
 *			that extends it derives from this struct, so the shared channels stay reachable through the interface.
 */
struct RenderContextEventHub {
	MulticastDispatcher<Events::VSyncEvent&> onChangeVSyncDispatcher;
};

/**
 * @class I_EventHubDispatcher
 * @brief The single root of the event hub, one per application
 * @details The hub is the pivot of the event system: raw platform callbacks arrive at the Receive* methods, are
 *			translated into engine events, and leave through the Dispatch* hooks to every subscriber. The five partial
 *			interfaces it inherits describe those hooks by family; this class exists so there is one type an
 *			application, a layer or the input state can hold instead of five, and so the subscribable channels can be
 *			reached without naming a backend.
 *
 *			That last part is what the accessors buy. Before them the channels lived in structs called
 *			mouseEventHub and mouseEventHub - identical in every member, different only in name - so every
 *			subscriber had to be written twice for what was the same code. The structs are now declared once above and
 *			handed out through GetMouseEventHub and friends, which is what lets a single Application subscribe on any
 *			backend.
 */
class CE_CORE_API I_EventHubDispatcher:
	public I_ApplicationEventHubDispatcher,
	public I_KeyboardEventHubDispatcher,
	public I_MouseEventHubDispatcher,
	public I_RenderContextEventHubDispatcher,
	public I_WindowEventHubDispatcher
{
public:
	~I_EventHubDispatcher() override = default;

public:
	/**
	 * @brief Gets the channels the application lifecycle events are delivered on
	 */
	[[nodiscard]] virtual ApplicationEventHub& GetApplicationEventHub() = 0;
	[[nodiscard]] virtual const ApplicationEventHub& GetApplicationEventHub() const = 0;

	/**
	 * @brief Gets the channels the keyboard events are delivered on
	 */
	[[nodiscard]] virtual KeyboardEventHub& GetKeyboardEventHub() = 0;
	[[nodiscard]] virtual const KeyboardEventHub& GetKeyboardEventHub() const = 0;

	/**
	 * @brief Gets the channels the mouse events are delivered on
	 */
	[[nodiscard]] virtual MouseEventHub& GetMouseEventHub() = 0;
	[[nodiscard]] virtual const MouseEventHub& GetMouseEventHub() const = 0;

	/**
	 * @brief Gets the channels the window events are delivered on
	 */
	[[nodiscard]] virtual WindowEventHub& GetWindowEventHub() = 0;
	[[nodiscard]] virtual const WindowEventHub& GetWindowEventHub() const = 0;

	/**
	 * @brief Gets the channels the render context events are delivered on
	 * @details Handed out as the shared struct even when the backend holds an extended one, so a subscriber that wants
	 *			a backend-specific channel has to reach for the concrete hub and say so.
	 */
	[[nodiscard]] virtual RenderContextEventHub& GetRenderContextEventHub() = 0;
	[[nodiscard]] virtual const RenderContextEventHub& GetRenderContextEventHub() const = 0;

public:
	/**
	 * @brief Gets which windowing backend feeds this hub
	 * @details Lets a subscriber that needs a backend-specific channel assert before downcasting, rather than guessing.
	 */
	[[nodiscard]] virtual Types::WindowApi GetWindowApi() const = 0;
};

template<Types::WindowApi Api>
class I_EventHubDispatcherBase: public I_EventHubDispatcher {
public:
	[[nodiscard]] Types::WindowApi GetWindowApi() const override { return _api; }

private:
	static constexpr Types::WindowApi _api = Api;
};

}

#endif //CE_CORE_HUB_EVENTS_I_EVENTHUBDISPATCHER_HPP
