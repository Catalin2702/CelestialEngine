cmake_minimum_required(VERSION 3.21)

# Create the CE_AppleMetal library (interface library since it only aggregates MetalBridge)
add_library(CE_AppleMetal INTERFACE)

# Add the MetalBridge subdirectory
add_subdirectory(MetalBridge)

# Link MetalBridge to CE_AppleMetal
target_link_libraries(CE_AppleMetal INTERFACE
	CE_MetalBridge
)

# Expose MetalBridge includes through CE_AppleMetal
target_include_directories(CE_AppleMetal INTERFACE
	MetalBridge/src/include
)
