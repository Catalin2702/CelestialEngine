cmake_minimum_required(VERSION 4.1)

include("${CMAKE_SOURCE_DIR}/Engine/config/cmake/CelestialEngineMacros.cmake")

project(CE_App LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 23)

add_executable(CE_App src/app.cpp)

target_link_libraries(CE_App PRIVATE
	CE_Config
	CelestialEngine
)
