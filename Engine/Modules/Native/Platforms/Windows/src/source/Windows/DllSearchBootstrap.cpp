//
// Module: CelestialEngine/Engine/Modules/Native/Platforms/Windows
// File: DllSearchBootstrap.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-18
// Updated by: Catalin Chirosca
// Updated: 2026-08-18
//

// Registers the executable's DLL/ subfolder as an additional DLL search directory before any
// delay-loaded CE_* module DLL is first touched. The engine DLLs live in DLL/ rather than next to
// the executable, and unlike Linux's RPATH the Windows loader has no notion of "also look in this
// subfolder relative to me" - it only searches the executable's own directory, system directories
// and PATH. Compiled directly into every executable that needs it (see ce_enable_dll_bootstrap in
// CelestialEngineMacros.cmake) so the registration below runs as a global static initializer,
// ahead of main(), regardless of which executable (CE_App or a Tests/*.exe) it ends up in.

#include <Windows.h>

#include <filesystem>
#include <system_error>

namespace {

	void RegisterDllSearchDirectory() {
		wchar_t exePath[MAX_PATH];
		const auto length = GetModuleFileNameW(nullptr, exePath, MAX_PATH);
		if (length == 0 || length == MAX_PATH)
			return;

		const auto exeDirectory = std::filesystem::path(exePath).parent_path();

		// CE_App.exe sits directly in Binaries/<toolchain>/<config>/, so its DLL/ folder is a
		// direct sibling; the Tests/*.exe sit one level deeper in .../Tests/, so theirs is one
		// level up. Probing both means every caller of ce_enable_dll_bootstrap "just works"
		// regardless of which subfolder it itself lives in.
		for (const auto& candidate: {exeDirectory / L"DLL", exeDirectory.parent_path() / L"DLL"}) {
			if (std::error_code errorCode; std::filesystem::is_directory(candidate, errorCode)) {
				SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
				AddDllDirectory(candidate.c_str());
				return;
			}
		}
	}

	const struct DllSearchBootstrap {
		DllSearchBootstrap() { RegisterDllSearchDirectory(); }
	} dllSearchBootstrap;

}
