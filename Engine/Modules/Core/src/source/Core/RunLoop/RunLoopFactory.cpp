//
// Module: CelestialEngine/Engine/Modules/Core/RunLoop
// File: RunLoopFactory.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-02
// Updated by: Catalin Chirosca
// Updated: 2026-09-02
//

#include "Core/RunLoop/I_RunLoop.hpp"
#include "Core/RunLoop/PacedRunLoop.hpp"
#include "Core/Window/I_Platform.hpp"


namespace CE::Core {

std::unique_ptr<I_RunLoop> I_RunLoop::MakeRunLoop(const I_Platform& platform) {
	switch (platform.GetWindowApi()) {
		// Cocoa is the one backend that takes the thread of control away: NSApplication::run() does not return, and
		// frames come back through a CAMetalDisplayLink instead of a loop we write. It gets its own implementation
		// alongside the Metal backend, once there is a CAMetalLayer for that link to attach to.

		// Everywhere else the thread is ours, and pacing it is the same std::chrono problem whatever created the
		// window - PacedRunLoop makes no windowing call at all, which is why it does not live under Platforms/.
		default:
			return std::make_unique<PacedRunLoop>();
	}
}

}
