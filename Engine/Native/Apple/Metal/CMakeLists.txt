cmake_minimum_required(VERSION 3.21)

# Create the CE_AppleMetal library (interface library since it only aggregates MetalBridge)
add_library(CE_AppleMetal INTERFACE)

# Add the MetalBridge subdirectory
add_subdirectory(MetalBridge)

# Link MetalBridge to CE_AppleMetal
target_link_libraries(CE_AppleMetal INTERFACE
	$<BUILD_INTERFACE:CE_MetalBridge>
	$<INSTALL_INTERFACE:CE_MetalBridge>
)

# Propagate include directories from CE_MetalBridge
target_include_directories(CE_AppleMetal INTERFACE
	$<TARGET_PROPERTY:CE_MetalBridge,INTERFACE_INCLUDE_DIRECTORIES>
)
