//
// Module: CelestialEngine/Engine/Modules/Core/Window/Platforms/Mac
// File: MacMenuBar.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-05
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#include "Core/Window/Platforms/Mac/MacMenuBar.hpp"

#include "Core/Application/Application.hpp"
#include "Core/Window/I_Window.hpp"
#include "Tools/Tools.hpp"
#include "Types/Types.hpp"
#include "Utility/Utility.hpp"

#include <AppKit/AppKit.hpp>
#include <Foundation/Foundation.hpp>

#include <string>


namespace CE::Core {

namespace {

/**
 * @brief Wraps a C string as an autoreleased NSString
 */
[[nodiscard]] NS::String* Text(const char* text) {
	return NS::String::string(text, NS::UTF8StringEncoding);
}

// --- the actions ----------------------------------------------------------
//
// NS::MenuItemCallback is a plain function pointer with no user-data slot, so a menu action cannot capture anything
// and has to find the engine on its own. Application::Get is how, and it is safe for the same reason every other
// call site is: a menu action can only be dispatched from inside the event pump, which only runs between Start and
// End, and the singleton is published for the whole of that.

void OnQuit(void*, SEL, const NS::Object*) {
	// Stop, not tear down: this is dispatched from inside the frame's event pump, so the loop has to finish the frame
	// it is in and unwind on its own. Start() calls End() once it returns - the same route the window's close button
	// takes.
	Application::Get().SetRunning(false);
}

void OnMiniaturize(void*, SEL, const NS::Object*) {
	Application::Get().GetWindow().Miniaturize();
}

void OnDeminiaturize(void*, SEL, const NS::Object*) {
	Application::Get().GetWindow().Deminiaturize();
}

void OnToggleFullScreen(void*, SEL, const NS::Object*) {
	Application::Get().GetWindow().ToggleFullScreen();
}

void OnToggleVSync(void*, SEL, const NS::Object*) {
	// Written back to the configuration as well as applied, because the configuration is what the run loop's pacing
	// and the next window are built from - applying it to the renderer alone would leave the two disagreeing until
	// something else wrote the flag.
	auto windowProps = Utility::Config::GetWindowProps();
	windowProps.VSync = not windowProps.VSync;
	Utility::Config::SetWindowProps(windowProps);

	Application::Get().SetVSync(windowProps.VSync);
}

/**
 * @brief Registers one menu action and hands back the selector that reaches it
 * @details registerActionCallback adds the method to NSObject itself, so the selector is answered by every object in
 *			the responder chain and the items need no target. That also means the names are global: they carry an
 *			engine prefix so they cannot collide with a selector AppKit already defines.
 */
[[nodiscard]] SEL Action(const char* name, const NS::MenuItemCallback callback) {
	return NS::MenuItem::registerActionCallback((std::string("CE_Menu") + name).c_str(), callback);
}

}

void InstallMacMenuBar() {
	const auto application = NS::Application::sharedApplication();
	if (not application) [[unlikely]] {
		CE_CORE_WARN("InstallMacMenuBar: there is no NSApplication to install a menu bar on; the menu bar is skipped.");
		return;
	}

	// Prepare() runs outside NSApplication::run(), so nothing else has a pool in place on this thread. Everything built
	// below is either autoreleased or retained by AppKit once the bar is installed, so draining at the end of this
	// function is safe - and without the pool the autoreleased halves would simply leak.
	const auto autoreleasePool = NS::TransferPtr(NS::AutoreleasePool::alloc()->init());

	const auto applicationName = NS::RunningApplication::currentApplication()->localizedName();

	const auto mainMenu = NS::TransferPtr(NS::Menu::alloc()->init());

	// --- application menu -------------------------------------------------
	// The first item of the bar is always the application's own, whatever its title says: macOS relabels it with the
	// process name and renders it in bold. That is why the submenu below is built without one.
	{
		const auto applicationMenuItem = NS::TransferPtr(NS::MenuItem::alloc()->init());
		const auto applicationMenu = NS::TransferPtr(NS::Menu::alloc()->init());

		const auto quitTitle = Text("Quit ")->stringByAppendingString(applicationName);
		const auto quitItem = applicationMenu->addItem(quitTitle, Action("Quit", OnQuit), Text("q"));
		quitItem->setKeyEquivalentModifierMask(NS::EventModifierFlagCommand);

		applicationMenuItem->setSubmenu(applicationMenu.get());
		mainMenu->addItem(applicationMenuItem.get());
	}

	// --- window menu ------------------------------------------------------
	{
		const auto windowMenuItem = NS::TransferPtr(NS::MenuItem::alloc()->init());
		const auto windowMenu = NS::TransferPtr(NS::Menu::alloc()->init(Text("Window")));

		// The two system-standard shortcuts, spelled the way macOS spells them elsewhere: Command-M minimises and
		// Control-Command-F toggles full screen. Deminiaturize has no standard key because the Dock is how a window
		// normally comes back, and VSync has none because it is ours rather than the system's.
		const auto miniaturizeItem = windowMenu->addItem(Text("Miniaturize"), Action("Miniaturize", OnMiniaturize), Text("m"));
		miniaturizeItem->setKeyEquivalentModifierMask(NS::EventModifierFlagCommand);

		windowMenu->addItem(Text("Deminiaturize"), Action("Deminiaturize", OnDeminiaturize), Text(""));

		const auto fullScreenItem = windowMenu->addItem(Text("Toggle Full Screen"), Action("ToggleFullScreen", OnToggleFullScreen), Text("f"));
		fullScreenItem->setKeyEquivalentModifierMask(
			static_cast<NS::KeyEquivalentModifierMask>(NS::EventModifierFlagControl | NS::EventModifierFlagCommand));

		windowMenu->addItem(Text("Toggle VSync"), Action("ToggleVSync", OnToggleVSync), Text(""));

		windowMenuItem->setSubmenu(windowMenu.get());
		mainMenu->addItem(windowMenuItem.get());
	}

	// Retains it, which is what lets everything above be released with the pool.
	application->setMainMenu(mainMenu.get());
}

}
