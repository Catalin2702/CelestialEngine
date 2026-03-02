
# Commentato fino a quando non ci saranno file di test
# set(SOURCES
# 	# CoreTests.cpp  # Aggiungi i file di test qui quando saranno pronti
# )
#
# add_executable(CE_TestsCore
# 	${SOURCES}
# )
#
# set_target_properties(CE_TestsCore PROPERTIES
# 	LINKER_LANGUAGE CXX
# )
#
# target_link_libraries(CE_TestsCore PRIVATE
# 	CE_Config
# 	CE_Core
#
# 	GTest::gtest
# 	GTest::gtest_main
# )
#
# add_test(NAME CoreTests
# 	COMMAND CE_TestsCore
# 	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Binaries/$<CONFIG>
# )
#
# set_tests_properties(CoreTests PROPERTIES
# 	LABELS "Engine;Core"
# )
