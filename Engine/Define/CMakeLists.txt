set(HEADERS
	src/include/Define/Bind.hpp
	src/include/Define/DynamicLinker.hpp
	src/include/Define/Event.hpp
	src/include/Define/Log.hpp
	src/include/Define/Type.hpp
)

add_library(CE_Define SHARED
	${HEADERS}
)

set_target_properties(CE_Define PROPERTIES
	LINKER_LANGUAGE CXX
)

target_link_libraries(CE_Define PUBLIC
	CE_Config
)

target_include_directories(CE_Define PUBLIC
	src/include
)
