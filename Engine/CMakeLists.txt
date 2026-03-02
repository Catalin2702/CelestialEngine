if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg_link/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

cmake_minimum_required(VERSION 3.21)

include("${CMAKE_SOURCE_DIR}/config/cmake/CelestialEngineMacros.cmake")

project(CelestialEngine LANGUAGES CXX VERSION 0.0.3)

set(HEADERS
	src/include/CelestialEngine.hpp
	src/internal/EntryPoint.hpp
)

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_subdirectory(Core)
add_subdirectory(Define)
add_subdirectory(Events)
add_subdirectory(Tools)
add_subdirectory(Types)
add_subdirectory(Window)

if(APPLE)
	add_subdirectory(Vendor/Apple/Metal/MetalCpp)
	add_subdirectory(Apple/MetalBridge)
endif()

add_library(CelestialEngine SHARED
	${HEADERS}
)

set_target_properties(CelestialEngine PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(CelestialEngine PUBLIC
	CE_Config
	CE_Core
	CE_Define
	CE_Events
	CE_Tools
	CE_Types
	CE_Window

	$<$<NOT:$<CONFIG:Dist>>:spdlog::spdlog>
)

if(APPLE)
	target_link_libraries(CelestialEngine PUBLIC
		CE_MetalCpp
		CE_MetalBridge
	)
endif()

target_include_directories(CelestialEngine
	PUBLIC src/include
	PUBLIC src/internal
)
