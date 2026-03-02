cmake_minimum_required(VERSION 3.21)

# Create the CE_Native library (interface library that aggregates platform-specific code)
add_library(CE_Native INTERFACE)

# Add platform-specific subdirectories based on the operating system
if(APPLE)
	add_subdirectory(Apple)
	target_link_libraries(CE_Native INTERFACE
		CE_Apple
	)
endif()

# Future platforms can be added here:
# if(WIN32)
# 	add_subdirectory(Windows)
# 	target_link_libraries(CE_Native INTERFACE CE_Windows)
# endif()
#
# if(UNIX AND NOT APPLE)
# 	add_subdirectory(Linux)
# 	target_link_libraries(CE_Native INTERFACE CE_Linux)
# endif()
