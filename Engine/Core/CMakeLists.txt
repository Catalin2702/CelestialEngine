cmake_minimum_required(VERSION 4.1)

project(CE_Core LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 23)

set(CORE_HEADERS
	src/include/Core/Core.hpp
	src/include/Core/Application.hpp
	../src/include/CelestialEngine.hpp
)

set(CORE_SOURCES
	src/source/Core/Application.cpp

)

add_library(CE_Core SHARED ${CORE_HEADERS} ${CORE_SOURCES})

target_link_libraries(CE_Core PRIVATE
	CE_Config
)

target_include_directories(CE_Core PUBLIC
	src/include
)

target_compile_definitions(CE_Core PRIVATE CE_CORE_EXPORTS)
