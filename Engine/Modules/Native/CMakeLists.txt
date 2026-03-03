cmake_minimum_required(VERSION 3.21)

add_library(CE_Native INTERFACE)

# Add platform-specific subdirectories based on the operating system
if(APPLE)
	add_subdirectory(Apple)
endif()

target_link_libraries(CE_Native INTERFACE
	CE_Config
)

# Platform-specific
target_link_libraries(CE_Native INTERFACE
	$<$<PLATFORM_ID:Darwin>:
		CE_Apple
	>
)
