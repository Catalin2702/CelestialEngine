set(HEADERS
	src/include/Define/DynamicLinker.hpp
	src/include/Define/Log.hpp
	src/include/Define/Type.hpp
	src/include/Define/Event.hpp
)

find_package(glfw3 CONFIG REQUIRED)

add_library(CE_Define SHARED
	${HEADERS}
)

set_target_properties(CE_Define PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(CE_Define
	PRIVATE CE_Config
)

target_compile_definitions(CE_Define PRIVATE CE_DEFINE_EXPORTS)

target_include_directories(CE_Define PUBLIC
	src/include
)
