//
// Module: CelestialEngine/Engine/Modules/Core
// File: Version.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-03
// Updated by: Catalin Chirosca
// Updated: 2026-11-03
//

#include "Core/Version.hpp"


namespace CE {

const char* GetEngineVersion() {
	return CE_ENGINE_VERSION;	// Engine/CMakeLists.txt's project() version, set on CE_Core by Modules/Core/CMakeLists.txt
}

}
