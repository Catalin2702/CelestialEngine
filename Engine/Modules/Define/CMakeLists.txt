#
# Module: CelestialEngine/Engine/Modules/Define
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-17
# Updated by: Catalin Chirosca
# Updated: 2026-03-23
#

set(HEADERS
	src/include/Define/Bind.hpp
	src/include/Define/DynamicLinker.hpp
	src/include/Define/Event.hpp
	src/include/Define/Log.hpp
	src/include/Define/Type.hpp
	src/include/Define/Window.hpp
	src/include/Define/Render.hpp
)

add_library(CE_Define INTERFACE
	${HEADERS}
)

set_target_properties(CE_Define PROPERTIES
	LINKER_LANGUAGE CXX
)

# Modules
target_link_libraries(CE_Define INTERFACE
	CE_Config
)

target_include_directories(CE_Define INTERFACE
	src/include
)

# Copy to Binaries/Last folder
ce_copy_to_last(CE_Define)

