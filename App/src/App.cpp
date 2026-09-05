//
// Module: CelestialEngine/App
// File: App.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-16
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

// The whole sandbox application, and it really is only this include: the umbrella header ends with
// EntryPoint.hpp, which defines main(), and main() constructs the one Application there is.
//
// There used to be a CreateApplication factory here, returning a GlfwApplication or a
// CocoaApplication behind an I_Application pointer. Both are gone: the window, the renderer and the
// run loop are chosen at runtime from the command line, so a single concrete class serves every
// backend and there is nothing left for a client to pick between.
//
// What a client adds is layers - pushed onto the application once it exists, not a subclass of it.

#include <CelestialEngine.hpp>
