//
// Module: CelestialEngine/Engine/Modules/Core/Window/Platforms/Mac/Cocoa
// File: CocoaWindow.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-16
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#include "Core/Window/Platforms/Mac/Cocoa/CocoaWindow.hpp"

#include "Core/Hub/Events/Platforms/Mac/Cocoa/CocoaEventHubDispatcher.hpp"
#include "Tools/Tools.hpp"
#include "Utility/Utility.hpp"

#include <AppKit/AppKit.hpp>
#include <CoreGraphics/CoreGraphics.h>
#include <MetalKit/MetalKit.hpp>

#include <objc/message.h>
#include <objc/runtime.h>

#include <stdexcept>
#include <string>
#include <utility>


namespace CE::Core {

namespace {

// metal-cpp exposes no NSUserDefaults wrapper, so the fullscreen flag is persisted through the Obj-C runtime directly. This
// mirrors what setFrameAutosaveName already does for the windowed frame, but for the native-fullscreen state (which AppKit only
// restores through the separate state-restoration machinery the engine does not opt into).

NS::String* FullScreenDefaultsKey(const NS::Window* window) {
	return window->title()->stringByAppendingString(NS::String::string(".fullscreen", NS::UTF8StringEncoding));
}

bool IsWindowFullScreen(const NS::Window* window) {
	return (window->styleMask() & NS::WindowStyleMaskFullScreen) != 0;
}

void SaveFullScreenFlag(NS::String* key, const bool value) {
	auto* const defaults = reinterpret_cast<id (*)(Class, SEL)>(objc_msgSend)(objc_getClass("NSUserDefaults"), sel_registerName("standardUserDefaults"));
	reinterpret_cast<void (*)(id, SEL, BOOL, id)>(objc_msgSend)(defaults, sel_registerName("setBool:forKey:"), value, reinterpret_cast<id>(key));
	// The engine stops the run loop and returns from main instead of going through NSApplication's normal termination, so force
	// the write to disk now rather than relying on the periodic flush.
	reinterpret_cast<void (*)(id, SEL)>(objc_msgSend)(defaults, sel_registerName("synchronize"));
}

bool LoadFullScreenFlag(NS::String* key) {
	auto* const defaults = reinterpret_cast<id (*)(Class, SEL)>(objc_msgSend)(objc_getClass("NSUserDefaults"), sel_registerName("standardUserDefaults"));
	return reinterpret_cast<BOOL (*)(id, SEL, id)>(objc_msgSend)(defaults, sel_registerName("boolForKey:"), reinterpret_cast<id>(key));
}

}

CocoaWindow::CocoaWindow() = default;

CocoaWindow::~CocoaWindow() {
	_Shutdown();
}

std::pair<u32, u32> CocoaWindow::GetWindowSize() const {
	auto* const window = _Native();
	if (not window) [[unlikely]]
		return {0, 0};

	const auto [origin, size] = window->frame();
	return {static_cast<u32>(size.width), static_cast<u32>(size.height)};
}

std::string CocoaWindow::GetTitle() const {
	auto* const window = _Native();
	if (not window) [[unlikely]]
		return "";

	return window->title()->utf8String();
}

std::pair<u32, u32> CocoaWindow::GetFrameSize() const {
	auto* const window = _Native();
	if (not window) [[unlikely]]
		return {0, 0};

	// The content rect is in points; the drawable is in backing pixels, which is the same thing scaled.
	const auto [origin, size] = window->contentRectForFrameRect();
	const auto scale = GetContentScale();

	return {static_cast<u32>(size.width * scale), static_cast<u32>(size.height * scale)};
}

f32 CocoaWindow::GetContentScale() const {
	auto* const window = _Native();
	if (not window) [[unlikely]]
		return 1.0f;

	return static_cast<f32>(window->backingScaleFactor());
}

u32 CocoaWindow::GetRefreshRate() const {
	auto* const window = _Native();
	if (not window) [[unlikely]]
		return 0;

	auto* const screen = window->screen();
	if (not screen) [[unlikely]]
		return 0;

	return static_cast<u32>(screen->maximumFramesPerSecond());
}

void CocoaWindow::SetTitle(const std::string_view title) {
	auto* const window = _Native();
	if (not window) [[unlikely]]
		return;

	// NS::String::string takes a C string, and a string_view carries no terminator of its own.
	const std::string terminated(title);
	window->setTitle(NS::String::string(terminated.c_str(), NS::UTF8StringEncoding));
}

void CocoaWindow::SetCursorMode(const Types::CursorMode cursorMode) {
	// NSCursor hide/unhide is reference counted: applying the same mode twice would leave the counter unbalanced and
	// the cursor hidden for good.
	if (_cursorMode == cursorMode) [[unlikely]]
		return;

	switch (cursorMode) {
		case Types::CursorMode::Normal: {
			CGAssociateMouseAndMouseCursorPosition(true);
			NS::Cursor::unhide();
			break;
		}
		case Types::CursorMode::Captured: {
			// Parked in the middle of the window first: the shortcut may be pressed with the cursor over the Dock or another
			// screen, and that is where it would otherwise reappear when the capture ends.
			if (auto* const window = _Native()) [[likely]] {
				const auto [origin, size] = window->frame();
				// AppKit counts y upwards from the bottom of the primary screen, CoreGraphics downwards from its top.
				const auto primaryHeight = CGDisplayBounds(CGMainDisplayID()).size.height;
				CGWarpMouseCursorPosition(CGPoint{
					.x = origin.x + (size.width * 0.5),
					.y = primaryHeight - (origin.y + (size.height * 0.5))
				});
			}

			NS::Cursor::hide();
			// Detaches the cursor from the mouse: the cursor stays put, while mouse events keep carrying deltaX/deltaY - the
			// motion without the position. Process-wide, which is why Input releases it whenever the window loses focus.
			CGAssociateMouseAndMouseCursorPosition(false);
			break;
		}
		case Types::CursorMode::Unknown:
		default: {
			// Rejected before it is stored: recording a mode nothing applied would make the next real change look like a
			// repeat to the guard above, and leave the hide/unhide count unbalanced.
			CE_CORE_WARN("CocoaWindow::SetCursorMode: Unknown cursor mode set.");
			return;
		}
	}

	_cursorMode = cursorMode;

	// Raised after the mode is applied, so subscribers learn what took effect rather than what was asked.
	if (_eventHub) [[likely]]
		_eventHub->ReceiveWindowCursorModeEvent(_cursorMode);
}

CA::MetalLayer* CocoaWindow::GetMetalLayer() const {
	auto* const window = _Native();
	if (not window) [[unlikely]]
		return nullptr;

	auto* const contentView = window->contentView();
	if (not contentView) [[unlikely]] {
		CE_CORE_WARN("CocoaWindow::GetMetalLayer: Cannot get the layer because the window has no content view yet.");
		return nullptr;
	}

	return contentView->layer();
}

void CocoaWindow::SetWindowSize(const u32 width, const u32 height) {
	auto* const window = _Native();
	if (not window) [[unlikely]]
		return;

	const auto [currentOrigin, currentSize] = window->frame();
	const CGRect frame = {
		.origin = currentOrigin,
		.size = {.width = static_cast<CGFloat>(width), .height = static_cast<CGFloat>(height)}
	};
	window->setFrame(frame, true, true);
}

void CocoaWindow::SetContentView(const MTK::View* view) const {
	auto* const window = _Native();
	if (not window) [[unlikely]]
		return;

	if (not view) [[unlikely]] {
		CE_CORE_ERROR("CocoaWindow::SetContentView: Cannot set a null content view!");
		throw std::runtime_error("CocoaWindow::SetContentView: Cannot set a null content view!");
	}

	window->setContentView(view);
	// The view must be the first responder to receive key events, and the window must accept mouse-moved events for the
	// view's dispatcher to get continuous cursor updates (there is no tracking area otherwise).
	(void)window->makeFirstResponder(view);
	window->setAcceptsMouseMovedEvents(true);
}

void CocoaWindow::Show() {
	auto* const window = _Native();
	if (not window) [[unlikely]]
		return;

	window->makeKeyAndOrderFront(nullptr);

	// Restore the native fullscreen state saved on the previous shutdown (the windowed frame itself is restored by the frame
	// autosave). Done after the window is on screen so toggleFullScreen animates into its own Space.
	if (LoadFullScreenFlag(FullScreenDefaultsKey(window)) and not IsWindowFullScreen(window))
		window->toggleFullScreen(nullptr);
}

void CocoaWindow::Init() {
	_InitWindow();
	_CreateView();

}

void CocoaWindow::_CreateView() {
	if (not _window) [[unlikely]] {
		constexpr auto error = "CocoaWindow::_CreateView: Cannot create the content view because the window is not initialized!";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	const auto& windowProps = Utility::Config::GetWindowProps();
	const CGRect frame = {
		.origin = {.x = static_cast<CGFloat>(0), .y = static_cast<CGFloat>(0)},
		.size = {.width = static_cast<CGFloat>(windowProps.width), .height = static_cast<CGFloat>(windowProps.height)}
	};

	// Created with no device, on purpose. MetalKit wants one only for the drawable and render-pass machinery the
	// engine does not use - MetalSwapchain owns the layer and vends its own drawables - and the layer's device is set
	// there, from the same MetalGraphicDevice the pipelines were compiled against. Asking for one here would mean the
	// window creating a second MTL::Device before the renderer even exists.
	_view = NS::TransferPtr(MTK::View::alloc()->init(frame, nullptr));
	if (not _view) [[unlikely]] {
		constexpr auto error = "CocoaWindow::_CreateView: Could not create the MetalKit view!";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	// The engine paces frames from its own run loop, so the view must not repaint on its own: neither on a timer nor
	// on setNeedsDisplay.
	_view->setPaused(true);
	_view->setEnableSetNeedsDisplay(false);

	// Input arrives here rather than at the window: AppKit sends key and mouse events to the first responder, which is
	// the view installed below.
	_view->setEventDispatcher(&viewEventDispatcher);

	_window->setContentView(_view.get());

	// First responder for key events, and mouse-moved delivery for continuous cursor updates - without it the view's
	// dispatcher only ever sees clicks, because there is no tracking area.
	(void)_window->makeFirstResponder(_view.get());
	_window->setAcceptsMouseMovedEvents(true);
}

void CocoaWindow::ConnectToEventHub(I_EventHubDispatcher& eventHub) {
	// The Receive* methods take NS::Notification*, so they are reachable only from the concrete hub - and a Cocoa
	// window can only ever be paired with a Cocoa hub.
	auto* const hub = dynamic_cast<CocoaEventHubDispatcher*>(&eventHub);
	if (not hub) [[unlikely]] {
		constexpr auto error = "CocoaWindow::ConnectToEventHub: a Cocoa window needs a Cocoa event hub!";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	_eventHub = hub;

	// The hub converts native (bottom-left) mouse positions out of this window's content view.
	hub->SetSources(this);

	using Hub = CocoaEventHubDispatcher;
	using NsEvent = EventDelegate<const NS::Event*>;
	using NsNotification = EventDelegate<const NS::Notification*>;

	auto& [willCloseDispatcher] = cocoaWindowEventDispatcher.nsWindowLifecycleEvents;
	auto& [didBecomeKeyDispatcher, didResignKeyDispatcher] = cocoaWindowEventDispatcher.nsWindowFocusEvents;

	willCloseDispatcher.Bind(NsNotification::FromMethod<Hub, &Hub::ReceiveWindowWillCloseEvent>(hub));

	// Focus changes feed the hub so the input state can release held keys once the window stops receiving events.
	didBecomeKeyDispatcher.Bind(NsNotification::FromMethod<Hub, &Hub::ReceiveWindowDidBecomeKeyEvent>(hub));
	didResignKeyDispatcher.Bind(NsNotification::FromMethod<Hub, &Hub::ReceiveWindowDidResignKeyEvent>(hub));

	auto& geometry = cocoaWindowEventDispatcher.nsWindowGeometryEvents;
	geometry.didResizeDispatcher.Bind(NsNotification::FromConstMethod<CocoaWindow, &CocoaWindow::_OnWindowDidResize>(this));

	// A move reports a size too, and it has to: dragging the window onto a display with a different backing scale
	// changes how many pixels the same number of points is worth, without the window ever being resized. Nothing else
	// tells us - AppKit sends no resize for it, and the view is not laid out again - so without this the drawable
	// follows the new scale while everything measured in points stays on the old one.
	geometry.didMoveDispatcher.Bind(NsNotification::FromConstMethod<CocoaWindow, &CocoaWindow::_OnWindowDidMove>(this));

	// Keyboard and mouse come off the view, not the window: AppKit delivers them to the first responder. These
	// bindings used to live in the application, against a dispatcher the render context owned - the same wires, moved
	// to where the view now is.
	auto& mouse = viewEventDispatcher.mouseEvents;

	// One handler per direction, three channels each: which button it was is read off the event, not off the channel.
	mouse.mouseDownDispatcher.Bind(NsEvent::FromMethod<Hub, &Hub::ReceiveMouseButtonDownEvent>(hub));
	mouse.rightMouseDownDispatcher.Bind(NsEvent::FromMethod<Hub, &Hub::ReceiveMouseButtonDownEvent>(hub));
	mouse.otherMouseDownDispatcher.Bind(NsEvent::FromMethod<Hub, &Hub::ReceiveMouseButtonDownEvent>(hub));

	mouse.mouseUpDispatcher.Bind(NsEvent::FromMethod<Hub, &Hub::ReceiveMouseButtonUpEvent>(hub));
	mouse.rightMouseUpDispatcher.Bind(NsEvent::FromMethod<Hub, &Hub::ReceiveMouseButtonUpEvent>(hub));
	mouse.otherMouseUpDispatcher.Bind(NsEvent::FromMethod<Hub, &Hub::ReceiveMouseButtonUpEvent>(hub));

	// A drag is a move with a button held, and AppKit reports it on its own channel rather than as a mouseMoved.
	mouse.mouseDraggedDispatcher.Bind(NsEvent::FromMethod<Hub, &Hub::ReceiveMouseDraggedEvent>(hub));
	mouse.rightMouseDraggedDispatcher.Bind(NsEvent::FromMethod<Hub, &Hub::ReceiveMouseDraggedEvent>(hub));
	mouse.otherMouseDraggedDispatcher.Bind(NsEvent::FromMethod<Hub, &Hub::ReceiveMouseDraggedEvent>(hub));

	mouse.mouseMovedDispatcher.Bind(NsEvent::FromMethod<Hub, &Hub::ReceiveMouseMovedEvent>(hub));
	mouse.scrollWheelDispatcher.Bind(NsEvent::FromMethod<Hub, &Hub::ReceiveScrollWheelEvent>(hub));

	auto& [keyDownDispatcher, keyUpDispatcher, flagsChangedDispatcher] = viewEventDispatcher.keyboardEvents;
	keyDownDispatcher.Bind(NsEvent::FromMethod<Hub, &Hub::ReceiveKeyDownEvent>(hub));
	keyUpDispatcher.Bind(NsEvent::FromMethod<Hub, &Hub::ReceiveKeyUpEvent>(hub));

	// Modifier keys never arrive as keyDown/keyUp: AppKit reports Shift, Ctrl, Alt and Cmd through flagsChanged only.
	flagsChangedDispatcher.Bind(NsEvent::FromMethod<Hub, &Hub::ReceiveFlagsChangedEvent>(hub));

	viewEventDispatcher.stateEvents.viewDidLayoutDispatcher.Bind(
		EventDelegate<>::FromConstMethod<CocoaWindow, &CocoaWindow::_OnViewDidLayout>(this)
	);
}

void CocoaWindow::_ReportSize() const {
	if (not _eventHub) [[unlikely]]
		return;

	const auto [windowWidth, windowHeight] = GetWindowSize();
	_eventHub->ReceiveWindowResizeEvent(windowWidth, windowHeight);

	// Backing pixels, and a separate event: this is the measurement a render target and ImGui's display size are in,
	// and on a Retina display it is not the same number as the one above.
	const auto [frameWidth, frameHeight] = GetFrameSize();
	_eventHub->ReceiveRenderResizeViewEvent(frameWidth, frameHeight);
}

void CocoaWindow::_OnWindowDidResize(const NS::Notification*) const {
	_ReportSize();
}

void CocoaWindow::_OnWindowDidMove(const NS::Notification*) const {
	// Fires all through a drag, and that is fine: _ReportSize is cheap, and the swapchain's Resize does nothing at all
	// when the numbers have not changed. The one move that matters is the one that crosses onto another screen.
	_ReportSize();
}

void CocoaWindow::_OnViewDidLayout() const {
	_ReportSize();
}

void CocoaWindow::Miniaturize() {
	if (auto* const window = _Native()) [[likely]]
		window->miniaturize(nullptr);
}

void CocoaWindow::Deminiaturize() {
	if (auto* const window = _Native()) [[likely]]
		window->deminiaturize(nullptr);
}

void CocoaWindow::ToggleFullScreen() {
	if (auto* const window = _Native()) [[likely]]
		window->toggleFullScreen(nullptr);
}

void CocoaWindow::_InitWindow() {
	const auto& windowProps = Utility::Config::GetWindowProps();
	const CGRect frame = {
		.origin = {.x = static_cast<CGFloat>(0), .y = static_cast<CGFloat>(0)},
		.size = {.width = static_cast<CGFloat>(windowProps.width), .height = static_cast<CGFloat>(windowProps.height)}
	};

	NS::Window* rawWindow = nullptr;
	try {
		rawWindow = NS::Window::alloc()->init(
			frame,
			NS::WindowStyleMaskTitled | NS::WindowStyleMaskClosable |
			NS::WindowStyleMaskResizable | NS::WindowStyleMaskMiniaturizable,
			NS::BackingStoreBuffered,
			false
		);
		if (not rawWindow) [[unlikely]] {
			CE_CORE_ERROR("CocoaWindow::_InitWindow: Could not create Cocoa window!");
			throw std::runtime_error("CocoaWindow::_InitWindow: Could not create Cocoa window!");
		}
	}
	catch (const std::exception& e) {
		CE_CORE_ERROR("CocoaWindow::_InitWindow: Exception while creating Cocoa window: {0}", e.what());
		throw;
	}
	catch (...) {
		CE_CORE_ERROR("CocoaWindow::_InitWindow: Unknown exception while creating Cocoa window");
		throw;
	}
	_window = NS::TransferPtr(rawWindow);

	_window->setDelegate(&cocoaWindowEventDispatcher);
	_window->setMinSize(CGSizeMake(640, 360));
	_window->setOpaque(false);
	_window->setTitle(NS::String::string(windowProps.title.c_str(), NS::UTF8StringEncoding));

	// Restore the saved window position BEFORE creating the Metal view
	_window->setFrameAutosaveName(_window->title());
}

void CocoaWindow::_Shutdown() {
	// A cursor still captured when the window goes would stay detached and hidden over every other application. Released
	// by hand rather than through SetCursorMode, which may log (and so throw) from a destructor and would report the
	// change to a hub that, being borrowed, may already be gone.
	if (_cursorMode == Types::CursorMode::Captured) [[unlikely]] {
		CGAssociateMouseAndMouseCursorPosition(true);
		NS::Cursor::unhide();
		_cursorMode = Types::CursorMode::Normal;
	}

	if (_window) [[unlikely]] {
		// Persist the native fullscreen state so Show() can restore it on the next launch (mirrors the frame autosave).
		SaveFullScreenFlag(FullScreenDefaultsKey(_window.get()), IsWindowFullScreen(_window.get()));

		_window->close();
		_window = nullptr;
	}
}

NS::Window* CocoaWindow::_Native(const std::source_location& location) const {
	if (not _window) [[unlikely]] {
		CE_CORE_WARN("CocoaWindow::{}: the window is not initialized.", location.function_name());
		return nullptr;
	}

	return _window.get();
}

}
