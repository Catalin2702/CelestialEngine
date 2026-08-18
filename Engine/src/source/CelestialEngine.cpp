//
// Module: Engine
// File: CelestialEngine.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-18
// Updated by: Catalin Chirosca
// Updated: 2026-08-18
//

// Deliberately not including CelestialEngine.hpp: it pulls in EntryPoint.hpp's int main(), which
// calls CE::Core::CreateApplication - a function only the client application defines. Linking that
// into CE_Engine.dll itself would leave it with an unresolved external. Define/DynamicLinker.hpp
// (for CE_ENGINE_API) is all this translation unit actually needs.
#include <Define/DynamicLinker.hpp>

namespace CE {
	CE_ENGINE_API const char* GetEngineVersion();
}

// CE_Engine is otherwise header-only (it just re-links the module DLLs together): with zero
// exported symbols, MSVC's link.exe (unlike lld-link, used via clang-cl/Ninja) silently skips
// generating CE_Engine.lib altogether, so every dependent target fails at link time with the
// Visual Studio generator. GetEngineVersion() is a genuinely useful export that also happens to
// give every generator at least one symbol to produce an import library for.
const char* CE::GetEngineVersion() {
	return CE_ENGINE_VERSION;
}
