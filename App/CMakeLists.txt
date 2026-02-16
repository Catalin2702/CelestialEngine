cmake_minimum_required(VERSION 4.1)

include("${CMAKE_SOURCE_DIR}/Engine/config/cmake/CelestialEngineMacros.cmake")

project(CE_App LANGUAGES CXX VERSION 0.0.1)

set(SOURCES
	src/app.cpp
)

add_executable(CE_App
	${SOURCES}
)

set_target_properties(CE_App PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(CE_App PRIVATE
	CE_Config
	CelestialEngine
)
