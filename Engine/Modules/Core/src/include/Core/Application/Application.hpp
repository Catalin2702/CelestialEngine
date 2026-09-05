//
// Module: CelestialEngine/Engine/Modules/Core/Application
// File: Application.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-02
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#pragma once

#ifndef CE_CORE_APPLICATION_APPLICATION_HPP
#define CE_CORE_APPLICATION_APPLICATION_HPP

#include "Core/Hub/Events/I_EventHubDispatcher.hpp"
#include "Core/Layers/LayerStack.hpp"
#include "Define/DynamicLinker.hpp"

#include <array>
#include <cstddef>
#include <memory>


namespace CE::Core {

class I_ImGuiLayer;
class I_IndexBuffer;
class I_PipelineState;
class I_Platform;
class I_Renderer;
class I_RunLoop;
class I_VertexBuffer;
class I_Window;

/**
 * @class Application
 * @brief The one application class, whatever the platform
 * @details What used to be GlfwApplication and CocoaApplication. Their public surfaces were already the same twelve
 *			overrides; what kept them apart was owning concrete window and context types, and owning the frame loop
 *			itself. Both are now held behind interfaces, so a single concrete class serves every backend and the
 *			polymorphism lives in the pieces rather than in the application.
 *
 *			The members are unique_ptr because the implementations are chosen at runtime, but nothing outside the
 *			member list ever handles a pointer: the accessors hand out references, and the class uses those on itself.
 */
class CE_CORE_API Application {
	using Clock = std::chrono::steady_clock;
	using TimePoint = std::chrono::time_point<Clock>;

	/// @brief Names the slots of _eventHubHandlers, so a subscription and its unsubscription cannot drift apart.
	enum EventHubSubscription: std::size_t {
		_WindowClose = 0,
		_WindowResize,
		_VSyncChange,

		_EventHubSubscriptionCount
	};

public:
	Application();

	Application(const Application&) = delete;
	Application(Application&&) noexcept = delete;

	virtual ~Application();

public:
	Application& operator = (const Application&) = delete;
	Application& operator = (Application&&) noexcept = delete;
public:
	/**
	 * @brief Adds a layer to the layer stack
	 * @param layer Pointer to the layer to add
	 * @details Layers are updated and receive events in the order they are pushed
	 */
	virtual void PushLayer(const std::shared_ptr<I_Layer>& layer);

	/**
	 * @brief Adds an overlay to the layer stack
	 * @param overlay Pointer to the overlay layer to add
	 * @details Overlays are rendered on top of regular layers and receive
	 *			events before regular layers
	 */
	virtual void PushOverlay(const std::shared_ptr<I_Layer>& overlay);

	/**
	 * @brief Removes a layer from the layer stack
	 * @param layer Pointer to the layer to remove
	 * @details Delegates to LayerStack::PopLayer, which removes the layer
	 *			from the stack and calls its OnDetach method
	 */
	virtual void PopLayer(const std::shared_ptr<I_Layer>& layer);

	/**
	 * @brief Removes an overlay from the layer stack
	 * @param overlay Pointer to the overlay layer to remove
	 * @details Delegates to LayerStack::PopOverlay, which removes the overlay
	 *			from the stack and calls its OnDetach method
	 */
	virtual void PopOverlay(const std::shared_ptr<I_Layer>& overlay);

	/**
	 * @brief Replaces a layer (or overlay) in the layer stack with a new one
	 * @param oldLayer Pointer to the layer to replace
	 * @param newLayer Pointer to the layer that takes its place
	 * @details Delegates to LayerStack::ReplaceLayer, which keeps the entry at its original index, calling the old
	 *			layer's OnDetach before the new layer's OnAttach
	 */
	virtual void ReplaceLayer(const std::shared_ptr<I_Layer>& oldLayer, const std::shared_ptr<I_Layer>& newLayer);


public:
	/**
	 * @brief Hands the thread of control to the run loop; returns when the loop ends
	 */
	void Start();

	void End();

	/**
	 * @brief One frame: simulate, render, present
	 */
	void Tick(f32 deltaTime);

	void Run() const;
	void Pause() const;

	void Init();

	void InitImguiLayer();

public:
	void SetRunning(bool running) const;

	void SetImGuiLayer(const std::shared_ptr<I_ImGuiLayer>& imguiLayer);

	void RemoveImGuiLayer();

public:
	[[nodiscard]] const I_Window& GetWindow() const { return *_window; }
	[[nodiscard]] I_Window& GetWindow() { return *_window; }

	[[nodiscard]] const I_Renderer& GetRenderer() const { return *_renderer; }
	[[nodiscard]] I_Renderer& GetRenderer() { return *_renderer; }

	[[nodiscard]] const I_RunLoop& GetRunLoop() const { return *_runLoop; }
	[[nodiscard]] I_RunLoop& GetRunLoop() { return *_runLoop; }

	[[nodiscard]] const I_EventHubDispatcher& GetEventHubDispatcher() const { return *_dispatcher; }
	[[nodiscard]] I_EventHubDispatcher& GetEventHubDispatcher() { return *_dispatcher; }

	[[nodiscard]] static Application& Get();
	[[nodiscard]] static const Application& GetConst();

protected:
	void _SubscribeToEventHubDispatcher();

	void _UnsubscribeFromEventHubDispatcher();

protected:
	void _SetEventHubDispatcher() const;

private:
	void _ResetDeltaTime() const;
	void _InitWindow();

	void _InitRenderer();

	/**
	 * @brief Builds everything that needs a live window, once the platform says there is one
	 * @details Bound to I_Platform::onReadyDispatcher and fired from Start(). This used to be the tail of the
	 *			constructor, which worked only because GLFW has a usable window the moment it is constructed; AppKit
	 *			does not, so the work moved to the moment both backends can name.
	 */
	void _OnPlatformReady();

	/**
	 * @brief Builds the ImGui layer the configured graphics API needs
	 * @details Split from InitImguiLayer because the two answer different questions: that one is the client asking
	 *			for an overlay, this one is the engine having something to build it against.
	 */
	void _MakeImGuiLayer();

private:
	void _OnFrame();

	void _OnLoopStarted() const;

	void _RenderFrame(f32 deltaTime);

	void _OnWindowClose(const Events::WindowCloseEvent& event) const;

	void _OnWindowResize(const Events::WindowResizeEvent& event) const;

	void _OnVSyncChange(const Events::VSyncEvent& event) const;

	/**
	 * @brief Works out what the run loop should be told to aim for
	 * @return u32 Frames per second, or 0 to let the loop run as fast as presentation allows
	 */
	[[nodiscard]] u32 _TargetFrameRate(bool vsync) const;

	/**
	 * @brief Tells the backend how far apart to hold successive frames, where it can
	 * @details Only Metal can do this, so only Metal is asked. It is the answer to wanting a high frame rate without
	 *			tearing: VSync blocks the loop on the display, presenting as soon as possible tears, and this holds
	 *			each frame back without blocking anything.
	 */
	void _ApplyPresentPacing(bool vsync) const;

public:
	/**
	 * @brief Switches presentation between waiting for the display's refresh and running free
	 * @details Not just a call through to the renderer: the run loop's pacing and every layer that shows the frame
	 *			rate have to learn about the change too, so it is announced on the hub and _OnVSyncChange does the
	 *			retargeting. That announcement used to come from the render context, which no longer exists.
	 */
	void SetVSync(bool enabled) const;

	/// @todo Renderer territory: an application should not know what a vertex buffer is.
	void _CreateRenderResources();

protected:
	std::unique_ptr<I_EventHubDispatcher> _dispatcher;

private:
	static std::atomic<Application*> _instance; ///< Singleton application instance

	std::unique_ptr<I_Platform> _platform;
	std::unique_ptr<I_Window> _window;
	std::unique_ptr<I_Renderer> _renderer;
	std::unique_ptr<I_RunLoop> _runLoop;

	/// @todo Renderer territory: an application should not know what a vertex buffer is.
	std::shared_ptr<I_PipelineState> _pipeline;
	std::shared_ptr<I_VertexBuffer> _vertexBuffer;
	std::shared_ptr<I_IndexBuffer> _indexBuffer;

	std::weak_ptr<I_ImGuiLayer> _imguiLayer;

	std::array<u32, _EventHubSubscriptionCount> _eventHubHandlers{};	///< Hub subscription tokens, indexed by the enum above

	/// @brief True once End() has torn the application down, so a second call does nothing.
	/// @details Not derived from the run loop: End() runs precisely because the loop has already stopped, so asking
	///			 the loop whether it is running would make the first call the one that gets skipped.
	/// Set by InitImguiLayer when it is called before the renderer exists - which is the normal case, since the
	/// entry point asks for the overlay between constructing the application and starting it.
	bool _imguiLayerRequested = false;

	bool _ended = false;
	mutable std::atomic<TimePoint> _lastFrameTime = Clock::now(); ///< Timestamp of the last frame for delta time calculation
	LayerStack _layerStack; ///< Stack of layers and overlays
};

}

#endif //CE_CORE_APPLICATION_APPLICATION_HPP
