# CelestialEngine

**CelestialEngine** è un game engine 3D amatoriale sviluppato in **C++23**, progettato per esplorare architetture di rendering moderne e sistemi di scripting ad alte prestazioni.

## 🔖 Version: 0.2.4

## 🚀 Key Features
* **Modern C++ Core:** Gestione della memoria tramite smart pointers e custom allocators.
* **RHI (`CE_RHI`):** modulo a sé con device, buffer, pipeline, shader e swapchain per OpenGL e Metal; il renderer in `CE_Core` lo usa solo tramite interfacce, e le finestre gli offrono una `RHI::I_Surface`.
* **Virtual File System (`CE_IO`):** percorsi `engine://`, `resources://` (sola lettura, file distribuiti con l'app) e `config://`, `cache://` (lettura/scrittura, per applicazione); errori tramite `std::expected`, scritture atomiche.

## 🛠 Tech Stack
* **Language:** C++23
* **Graphics API:** OpenGL, Metal
* **Build System:** CMake.
* **Third Party:** metal-cpp, spdlog, glfw3, glad, glm, Dear ImGui, stb, GoogleTest

## 📂 Project Structure
```text
├── CMakeLists.txt          # Top level: resolves the backends, includes Engine/cmake, adds the engine and the apps
├── CMakePresets.json       # Shared presets, one per toolchain (CMakeUserPresets.json: local variants)
├── Apps/
│   └── Sandbox/            # The engine's test bed: add_executable() + one ce_configure_application() call
├── Engine/
│   ├── cmake/              # Build machinery: CE_Config, output layout, ce_configure_application, backend selection
│   │   └── templates/      # Default Info.plist and entitlements of every application
│   ├── Modules/            # Core, Define, Events, IO, Native, RHI, Tools, Types, Utility
│   ├── Shaders/            # Metal and GLSL shaders, compiled per selected backend
│   ├── src/
│   │   ├── include/        # CelestialEngine.hpp, the header an application includes
│   │   └── source/         # EntryPoint.cpp - main(), added to each app by ce_configure_application: do NOT define your own
│   ├── Tests/              # GoogleTest suites, one per module (CE_BUILD_TESTS)
│   ├── Vendor/             # CMake wrappers of the third-party libraries
│   └── Submodules/         # ImGui, metal-cpp
├── Scripts/                # Build-all, static analysis, Xcode project, maintenance (bump_updated_dates.sh)
└── .exclude/               # Everything generated, git-ignored: build/<preset> (outputs in Out/<config>), tmp/
```
