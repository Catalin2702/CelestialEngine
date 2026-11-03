# CelestialEngine

**CelestialEngine** è un game engine 3D amatoriale sviluppato in **C++23**, progettato per esplorare architetture di rendering moderne e sistemi di scripting ad alte prestazioni.

## 🔖 Version: 0.1.3

## 🚀 Key Features
* **Modern C++ Core:** Gestione della memoria tramite smart pointers e custom allocators.

## 🛠 Tech Stack
* **Language:** C++23
* **Graphics API:** OpenGL, Metal
* **Build System:** CMake.
* **Third Party:** metal-cpp, spdlog, glfw3, glad

## 📂 Project Structure
```text
├── CMakeLists.txt          # Top level: resolves the backends, includes Engine/cmake, adds the engine and the apps
├── CMakePresets.json       # Shared presets, one per toolchain (CMakeUserPresets.json: local variants)
├── Apps/
│   └── Sandbox/            # The engine's test bed: add_executable() + one ce_configure_application() call
├── Engine/
│   ├── cmake/              # Build machinery: CE_Config, Binaries/ layout, ce_configure_application, backend selection
│   │   └── templates/      # Default Info.plist and entitlements of every application
│   ├── Modules/            # Core, Define, Events, Native, Tools, Types, Utility
│   ├── Shaders/            # Metal and GLSL shaders, compiled per selected backend
│   ├── src/
│   │   ├── include/        # CelestialEngine.hpp, the header an application includes
│   │   └── internal/       # EntryPoint.hpp - defines main(): do NOT define your own
│   ├── Tests/              # GoogleTest suites, one per module (CE_BUILD_TESTS)
│   ├── Vendor/             # CMake wrappers of the third-party libraries
│   └── Submodules/         # ImGui, metal-cpp
├── Scripts/                # Build-all, run-all, static analysis, Xcode project
└── .exclude/               # Everything generated, git-ignored: build/<preset>, Binaries/<toolchain>/<config>, tmp/
```
