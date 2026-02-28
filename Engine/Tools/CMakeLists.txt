set(HEADERS
	src/include/Tools/CMD/Window.hpp
	src/include/Tools/Log/Log.hpp
	src/include/Tools/Manipulation/String.hpp
	src/include/Tools/Time/Timer.hpp
	src/include/Tools/ImGui/ImGui.hpp
)

set(SOURCES
	src/source/Tools/CMD/Window.cpp
	src/source/Tools/Log/Log.cpp
	src/source/Tools/Manipulation/String.cpp
	src/source/Tools/Time/Timer.cpp
	src/source/Tools/ImGui/ImGui.cpp
)

add_library(CE_Tools SHARED
	${HEADERS}
	${SOURCES}
)

set_target_properties(CE_Tools PROPERTIES
	LINKER_LANGUAGE CXX
)

target_link_libraries(CE_Tools PUBLIC
	CE_Config
	CE_Define
	CE_Types
)

# Link spdlog only for Debug and Release builds, not for Dist
if(NOT CMAKE_BUILD_TYPE STREQUAL "Dist")
	target_link_libraries(CE_Tools PUBLIC spdlog::spdlog)
endif()

target_include_directories(CE_Tools PUBLIC
	src/include
	../Core/src/include
	../Define/src/include
	../Types/src/include
	../Window/src/include
)
