//
// Module: CelestialEngine/Engine/Modules/Core/Layers/Scene
// File: SceneLayer.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-08
// Updated by: Catalin Chirosca
// Updated: 2026-09-08
//

#pragma once

#ifndef CE_CORE_LAYERS_SCENE_SCENELAYER_HPP
#define CE_CORE_LAYERS_SCENE_SCENELAYER_HPP

#include "Core/Layers/I_Layer.hpp"
#include "Core/Render/Camera/CameraRig.hpp"
#include "Define/DynamicLinker.hpp"
#include "Utility/Delegate/Dispatcher.hpp"

#include <array>


namespace CE::Events {
	class AppTickEvent;
	class KeyPressedEvent;
	class KeyReleasedEvent;
	class MouseButtonPressedEvent;
	class MouseButtonReleasedEvent;
	class MouseDraggedEvent;
	class MouseMovedEvent;
	class MouseWheelScrolledEvent;
	class WindowResizeEvent;
}

namespace CE::Core {

/**
 * @class SceneLayer
 * @brief The layer that owns the point of view: a camera, and whatever drives it
 * @details Sits between the input the hub delivers and the matrices the renderer draws from. It owns a CameraRig -
 *			the camera plus its controller - feeds that rig the frame's input and delta, and hands the resulting
 *			CameraData to the renderer once per frame.
 *
 *			It draws nothing itself. What to draw belongs to whoever owns the geometry, which is why OnRender only
 *			fires the delegate SetOnRenderDelegate was given: the layer supplies the point of view, the client
 *			supplies the scene.
 *
 *			Every hub channel it wants exists on I_EventHubDispatcher, so unlike the ImGui layers it names no backend
 *			and works unchanged on Metal and OpenGL.
 */
class CE_CORE_API SceneLayer final: public I_Layer {
	enum EventHubSubscription: std::size_t {
		Tick = 0,
		MouseMoved,
		MouseDragged,
		MouseWheelScrolled,
		MouseButtonPressed,
		MouseButtonReleased,
		KeyboardKeyPressed,
		KeyboardKeyReleased,
		WindowResize,
		_Count
	};

public:
	/**
	 * @brief Builds the layer and the rig it drives
	 * @param cameraDescriptor Where the camera starts and what it looks at
	 * @param cameraProjectionDescriptor Which lens it looks through
	 * @param cameraControllerDescriptor What moves it
	 */
	SceneLayer(const CameraDescriptor& cameraDescriptor,
		const CameraProjectionDescriptor& cameraProjectionDescriptor,
		const CameraControllerDescriptor& cameraControllerDescriptor
	);

	SceneLayer(const SceneLayer&) = delete;
	SceneLayer(SceneLayer&&) noexcept = default;

	/// Nothing to unwind by hand: the Subscription tokens leave the hub as they are destroyed.
	~SceneLayer() override = default;

public:
	SceneLayer& operator = (const SceneLayer&) = delete;
	SceneLayer& operator = (SceneLayer&&) noexcept = default;

public:
	void OnAttach() override {}
	void OnDetach() override {}

	/**
	 * @brief Advances the rig by the last frame's delta and publishes the resulting point of view
	 * @details Runs before the renderer opens the frame, so the matrices the pass draws with are this frame's, not
	 *			the previous one's.
	 */
	void OnUpdate() override;

	/**
	 * @brief Fires the client's render delegate
	 */
	void OnRender() const override;

public:
	void SubscribeToEventHub(I_EventHubDispatcher& eventHubDispatcher) override;
	void UnsubscribeFromEventHub() override;

public:
	/**
	 * @brief Sets what the layer calls when it is asked to render
	 * @param onRender The delegate to invoke from OnRender; a default-constructed one clears it
	 */
	void SetOnRenderDelegate(const EventDelegate<>& onRender);

public:
	[[nodiscard]] const CameraRig& GetCameraRig() const { return _cameraRig; }
	[[nodiscard]] CameraRig& GetCameraRig() { return _cameraRig; }

private:
	/// Records the frame delta the tick channel carries. I_Layer::OnUpdate takes no argument, and the hub is where
	/// the number already travels, so the layer reads it here rather than reaching for a clock of its own.
	void _OnTick(const Events::AppTickEvent& event);

	/// Keeps the projection's aspect ratio in step with the window. Not const: it writes through to the projection.
	void _OnWindowResize(const Events::WindowResizeEvent& event);

	// One per channel, because a delegate binds an exact signature. Each one does the same thing - hand the event to
	// the controller and consume it if the controller acted on it - so what a controller reacts to is decided in the
	// controller, not here.
	void _OnMouseMoved(Events::MouseMovedEvent& event) const;
	void _OnMouseDragged(Events::MouseDraggedEvent& event) const;
	void _OnMouseScrolled(Events::MouseWheelScrolledEvent& event) const;
	void _OnMouseButtonPressed(Events::MouseButtonPressedEvent& event) const;
	void _OnMouseButtonReleased(Events::MouseButtonReleasedEvent& event) const;
	void _OnKeyPressed(Events::KeyPressedEvent& event) const;
	void _OnKeyReleased(Events::KeyReleasedEvent& event) const;

	/// Offers one event to the controller and consumes it if the controller took it.
	void _ForwardToController(Events::I_Event& event) const;

private:
	UnicastDispatcher<> _OnRender;					///< What the client wants drawn; unbound until SetOnRenderDelegate

	CameraRig _cameraRig;

	f32 _deltaTime = 0.0_f32;						///< Last delta the tick channel reported, in seconds

	/// Owning tokens, ordered to match the enum above.
	std::array<Subscription, _Count> _subscriptions;
};

}

#endif //CE_CORE_LAYERS_SCENE_SCENELAYER_HPP
