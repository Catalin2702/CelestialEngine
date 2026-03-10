# CelestialEngine

**CelestialEngine** è un game engine 3D amatoriale sviluppato in **C++23**, progettato per esplorare architetture di rendering moderne e sistemi di scripting ad alte prestazioni.

## 🔖 Version: 0.0.6

## 🚀 Key Features
* **Modern C++ Core:** Gestione della memoria tramite smart pointers e custom allocators.

## 🛠 Tech Stack
* **Language:** C++23
* **Graphics API:** OpenGL, Metal
* **Build System:** CMake.
* **Third Party:** metal-cpp, spdlog, glfw3, glad

## 📂 Project Structure
```text
├── App/
└── Engine/
	├── config/
		└── cmake # Cmake global configs
	├── Core/
		├── Core/ # Application class to inherit to change app behaviour
		└── Layers/ # Layer and StackLayer
	├── Define/
	├── Events/ # Application events manager
	├── src/
		├── CelestialEngine.hpp # Header to include in the app
		└── EntryPoint.hpp # Entry point with main function (DO NOT DEFINE your main function)
	├── Tools/
		├── CMD/ # Tools to run the app through CMD
		├── Log/
		├── Manipulation/ # String manipulation
		└── Time/
	├── Types/
		└── Window/
	├── Vendor
		└── Apple
			└── Metal
				└── MetalCpp/ # Official and custom parts from metal-cpp
	└── Window
		└── Platforms
			├── Linux
			├── Mac/ # Metal implementation in GLFW
			├── Universal/ # OpenGL implementation in GLFW
			└── Windows
```
