//
// Module: CelestialEngine/Engine/Modules/Core/Layers/Scene
// File: SceneLayer.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-08
// Updated by: Catalin Chirosca
// Updated: 2026-09-08
//

#include "Core/Layers/Scene/SceneLayer.hpp"

#include "Core/Application/Application.hpp"
#include "Core/Hub/Events/I_EventHubDispatcher.hpp"
#include "Core/Render/Renderer/I_Renderer.hpp"

#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Events/WindowEvent.hpp"


namespace CE::Core {

SceneLayer::SceneLayer(const CameraDescriptor& cameraDescriptor, const CameraProjectionDescriptor& cameraProjectionDescriptor,
	const CameraControllerDescriptor& cameraControllerDescriptor):
	I_Layer("SceneLayer"),
	_cameraRig(cameraDescriptor, cameraProjectionDescriptor, cameraControllerDescriptor) {}

void SceneLayer::OnUpdate() {
	_cameraRig.OnUpdate(_deltaTime);

	Application::Get().GetRenderer().SetCameraData(_cameraRig.GetCameraData());
}

void SceneLayer::OnRender() const {
	_OnRender.Dispatch();
}

void SceneLayer::SubscribeToEventHub(I_EventHubDispatcher& eventHubDispatcher) {
	UnsubscribeFromEventHub();

	auto& applicationEventHub = eventHubDispatcher.GetApplicationEventHub();
	auto& keyboardEventHub = eventHubDispatcher.GetKeyboardEventHub();
	auto& mouseEventHub = eventHubDispatcher.GetMouseEventHub();
	auto& windowEventHub = eventHubDispatcher.GetWindowEventHub();

	_subscriptions[Tick] = applicationEventHub.onTickMulticastDispatcher.SubscribeScoped(EventDelegate<Events::AppTickEvent&>::FromMethod<SceneLayer, &SceneLayer::_OnTick>(this));

	_subscriptions[MouseMoved] = mouseEventHub.onMovedMulticastDispatcher.SubscribeScoped(EventDelegate<Events::MouseMovedEvent&>::FromConstMethod<SceneLayer, &SceneLayer::_OnMouseMoved>(this));
	_subscriptions[MouseDragged] = mouseEventHub.onDraggedMulticastDispatcher.SubscribeScoped(EventDelegate<Events::MouseDraggedEvent&>::FromConstMethod<SceneLayer, &SceneLayer::_OnMouseDragged>(this));
	_subscriptions[MouseWheelScrolled] = mouseEventHub.onWheelScrolledMulticastDispatcher.SubscribeScoped(EventDelegate<Events::MouseWheelScrolledEvent&>::FromConstMethod<SceneLayer, &SceneLayer::_OnMouseScrolled>(this));
	_subscriptions[MouseButtonPressed] = mouseEventHub.onButtonPressedMulticastDispatcher.SubscribeScoped(EventDelegate<Events::MouseButtonPressedEvent&>::FromConstMethod<SceneLayer, &SceneLayer::_OnMouseButtonPressed>(this));
	_subscriptions[MouseButtonReleased] = mouseEventHub.onButtonReleasedMulticastDispatcher.SubscribeScoped(EventDelegate<Events::MouseButtonReleasedEvent&>::FromConstMethod<SceneLayer, &SceneLayer::_OnMouseButtonReleased>(this));

	_subscriptions[KeyboardKeyPressed] = keyboardEventHub.onPressedMulticastDispatcher.SubscribeScoped(EventDelegate<Events::KeyPressedEvent&>::FromConstMethod<SceneLayer, &SceneLayer::_OnKeyPressed>(this));
	_subscriptions[KeyboardKeyReleased] = keyboardEventHub.onReleasedMulticastDispatcher.SubscribeScoped(EventDelegate<Events::KeyReleasedEvent&>::FromConstMethod<SceneLayer, &SceneLayer::_OnKeyReleased>(this));

	_subscriptions[WindowResize] = windowEventHub.onResizeMulticastDispatcher.SubscribeScoped(EventDelegate<Events::WindowResizeEvent&>::FromMethod<SceneLayer, &SceneLayer::_OnWindowResize>(this));
}

void SceneLayer::UnsubscribeFromEventHub() {
	// Assigning an empty array over the live one runs every token's Reset: no hub to reach for, and nothing to keep
	// in step with the enum by hand.
	_subscriptions = {};
}

void SceneLayer::SetOnRenderDelegate(const EventDelegate<>& onRender) {
	// Unbind first: Bind refuses to overwrite a live binding, and replacing the render delegate is a legitimate thing
	// to want - a client that swaps scenes should not have to know a previous one is still attached.
	_OnRender.Unbind();
	_OnRender.Bind(onRender);
}

void SceneLayer::_OnTick(const Events::AppTickEvent& event) {
	_deltaTime = event.GetDeltaTime();
}

void SceneLayer::_OnWindowResize(const Events::WindowResizeEvent& event) {
	// Zero on a minimised window, which would make the aspect ratio a division by zero.
	if (event.GetWidth() == 0 or event.GetHeight() == 0) [[unlikely]]
		return;

	_cameraRig.GetCamera().GetProjection().SetViewportSize(event.GetWidth(), event.GetHeight());
}

void SceneLayer::_OnMouseMoved(Events::MouseMovedEvent& event) const { _ForwardToController(event); }
void SceneLayer::_OnMouseDragged(Events::MouseDraggedEvent& event) const { _ForwardToController(event); }
void SceneLayer::_OnMouseScrolled(Events::MouseWheelScrolledEvent& event) const { _ForwardToController(event); }
void SceneLayer::_OnMouseButtonPressed(Events::MouseButtonPressedEvent& event) const { _ForwardToController(event); }
void SceneLayer::_OnMouseButtonReleased(Events::MouseButtonReleasedEvent& event) const { _ForwardToController(event); }
void SceneLayer::_OnKeyPressed(Events::KeyPressedEvent& event) const { _ForwardToController(event); }
void SceneLayer::_OnKeyReleased(Events::KeyReleasedEvent& event) const { _ForwardToController(event); }

void SceneLayer::_ForwardToController(Events::I_Event& event) const {
	if (_cameraRig.OnEvent(event))
		event.Consume();
}

}
