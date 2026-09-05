#
# Module: CelestialEngine/Engine/Modules/Define
# File: CMakeLists.txt
# Created by: Catalin Chirosca
# Created: 2026-02-17
# Updated by: Catalin Chirosca
# Updated: 2026-09-05
#

set(HEADERS
	src/include/Define/Bind.hpp
	src/include/Define/Config.hpp
	src/include/Define/DynamicLinker.hpp
	src/include/Define/Event.hpp
	src/include/Define/Log.hpp
	src/include/Define/Types.hpp
	src/include/Define/Render.hpp

	# Main
	src/include/Define/Define.hpp
)

add_library(CE_Define INTERFACE
	${HEADERS}
)

set_target_properties(CE_Define PROPERTIES
	LINKER_LANGUAGE CXX
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION}
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

