if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg_link/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

if (NOT TARGET CE_Config)
	add_library(CE_Config INTERFACE)

	target_compile_definitions(CE_Config INTERFACE
		$<$<CONFIG:Debug>:
		DEBUG_BUILD
		>

		$<$<PLATFORM_ID:Windows>:
		CE_PLATFORM_WINDOWS
		>
		$<$<PLATFORM_ID:Darwin>:
		CE_PLATFORM_MACOS
		>
		$<$<PLATFORM_ID:Linux>:
		CE_PLATFORM_LINUX
		>
	)

	target_compile_features(CE_Config INTERFACE
		cxx_std_23
	)

	target_compile_options(CE_Config INTERFACE
		$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:
		-Wall
		-Wextra
		-Werror
		>
		$<$<CXX_COMPILER_ID:MSVC>:
		/W4
		/WX
		/permissive-
		>

		$<$<CONFIG:Debug>:
		-O0
		-g
		>
		$<$<CONFIG:Release>:
		-O3
		>
	)
endif ()
