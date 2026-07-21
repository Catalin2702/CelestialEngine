//
// Module: CelestialEngine/Engine/Modules/Core/Application/Platforms/Mac
// File: MacMenuBar.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-21
// Updated by: Catalin Chirosca
// Updated: 2026-07-21
//

#pragma once

#ifndef CE_CORE_APPLICATION_PLATFORMS_MAC_MACMENUBAR_HPP
#define CE_CORE_APPLICATION_PLATFORMS_MAC_MACMENUBAR_HPP

#include <AppKit/AppKit.hpp>

#include <string>

namespace CE::Core {

/**
 * @brief Builds the macOS main menu bar wired to a specific application's static menu callbacks
 * @tparam TApp Application type exposing the StOn*Callback static methods (CocoaApplication, GlfwApplication, ...)
 * @return NS::Menu* Autoreleased main menu ready to be handed to NS::Application::setMainMenu
 * @details Shared by every macOS-hosted application backend. Because GLFW still creates a real NSApplication on macOS, the same
 *			menu (and the same callback contract) is reused for both the Cocoa and GLFW backends; each backend supplies its own
 *			StOn*Callback implementations, which this template resolves at instantiation.
 */
template <typename TApp>
NS::Menu* CreateMenuBar() {
	const auto mainMenu = NS::Menu::alloc()->init();

	const auto appName = NS::RunningApplication::currentApplication()->localizedName();
	const auto appNameString = std::string(appName->cString(NS::UTF8StringEncoding));

#pragma region AppMenu
	const auto appMenuItem = NS::MenuItem::alloc()->init();
	const auto appMenu = NS::Menu::alloc()->init();

#pragma region SectionQuit
	const auto quitTitle = NS::String::string("Quit ", NS::UTF8StringEncoding)->stringByAppendingString(appName);
	const auto quitSelector = NS::MenuItem::registerActionCallback((appNameString + "Quit").c_str(), TApp::StOnQuitMenuCallback);
	appMenu->addItem(quitTitle, quitSelector, NS::String::string("q", NS::UTF8StringEncoding));
#pragma endregion

	appMenuItem->setSubmenu(appMenu);
	mainMenu->addItem(appMenuItem);

	appMenu->release();
	appMenuItem->release();
#pragma endregion

#pragma region WindowMenu
	const auto windowMenuItem = NS::MenuItem::alloc()->init();
	const auto windowMenu = NS::Menu::alloc()->init(NS::String::string("Window", NS::UTF8StringEncoding));

#pragma region SectionMiniaturize
	const auto miniaturizeTitle = NS::String::string("Miniaturize", NS::UTF8StringEncoding);
	const auto miniaturizeSelector = NS::MenuItem::registerActionCallback((appNameString + "Miniaturize").c_str(), TApp::StOnMiniaturizeCallback);
	windowMenu->addItem(miniaturizeTitle, miniaturizeSelector, NS::String::string("", NS::UTF8StringEncoding));
#pragma endregion

#pragma region SectionDeminiaturize
	const auto deminiaturizeTitle = NS::String::string("Deminiaturize", NS::UTF8StringEncoding);
	const auto deminiaturizeSelector = NS::MenuItem::registerActionCallback((appNameString + "Deminiaturize").c_str(), TApp::StOnDeminiaturizeCallback);
	windowMenu->addItem(deminiaturizeTitle, deminiaturizeSelector, NS::String::string("", NS::UTF8StringEncoding));
#pragma endregion

#pragma region SectionFullscreen
	const auto toggleFullscreenTitle = NS::String::string("Toggle Fullscreen", NS::UTF8StringEncoding);
	const auto toggleFullscreenSelector = NS::MenuItem::registerActionCallback((appNameString + "ToggleFullscreen").c_str(), TApp::StOnToggleFullscreenCallback);
	windowMenu->addItem(toggleFullscreenTitle, toggleFullscreenSelector, NS::String::string("", NS::UTF8StringEncoding));
#pragma endregion

#pragma region SectionVSync
	const auto vsyncTitle = NS::String::string("Toggle VSync", NS::UTF8StringEncoding);
	const auto vsyncSelector = NS::MenuItem::registerActionCallback((appNameString + "ToggleVSync").c_str(), TApp::StOnToggleVSyncCallback);
	windowMenu->addItem(vsyncTitle, vsyncSelector, NS::String::string("", NS::UTF8StringEncoding));
#pragma endregion

	windowMenuItem->setSubmenu(windowMenu);
	mainMenu->addItem(windowMenuItem);

	windowMenu->release();
	windowMenuItem->release();
#pragma endregion

	return mainMenu->autorelease();
}

}

#endif //CE_CORE_APPLICATION_PLATFORMS_MAC_MACMENUBAR_HPP
