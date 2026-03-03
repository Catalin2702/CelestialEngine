cmake_minimum_required(VERSION 3.21)

# Create the CE_Apple library (interface library since it aggregates platform-specific APIs)
add_library(CE_Apple INTERFACE)

# Add the Metal subdirectory
add_subdirectory(Metal)

# Link Metal to CE_Apple
target_link_libraries(CE_Apple INTERFACE
	CE_AppleMetal
)
