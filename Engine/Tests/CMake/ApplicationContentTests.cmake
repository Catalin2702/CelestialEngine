#
# Module: CelestialEngine/Engine/Tests/CMake
# File: ApplicationContentTests.cmake
# Created by: Catalin Chirosca
# Created: 2026-11-26
# Updated by: Catalin Chirosca
# Updated: 2026-11-26
#

# ce_collect_application_content (Engine/CMake/CEApplication.cmake), run as a script: builds a fake content folder and
# checks which of its files the function keeps. A failed check stops with FATAL_ERROR, which is what fails the test.

include("${CE_ENGINE_CMAKE_DIR}/CEApplication.cmake")

function(expect_equal actual expected what)
	if (NOT "${actual}" STREQUAL "${expected}")
		message(FATAL_ERROR "${what}\n  expected: '${expected}'\n  actual:   '${actual}'")
	endif ()
endfunction()

file(REMOVE_RECURSE "${WORK_DIR}")

# Kept: files at the root and in nested folders.
file(WRITE "${WORK_DIR}/Content/Welcome.txt" "hello")
file(WRITE "${WORK_DIR}/Content/Textures/Wall.png" "png")
file(WRITE "${WORK_DIR}/Content/Textures/Deep/Floor.png" "png")

# Left out: hidden files, and every file inside a hidden folder.
file(WRITE "${WORK_DIR}/Content/.DS_Store" "finder")
file(WRITE "${WORK_DIR}/Content/Textures/.DS_Store" "finder")
file(WRITE "${WORK_DIR}/Content/.git/config" "git")

ce_collect_application_content("${WORK_DIR}/Content" files)

# Relative to the folder, sorted (byte order: upper case before lower case).
expect_equal("${files}" "Textures/Deep/Floor.png;Textures/Wall.png;Welcome.txt"
	"ce_collect_application_content: wrong files")

# An empty folder is an empty list, not an error.
file(MAKE_DIRECTORY "${WORK_DIR}/Empty")
ce_collect_application_content("${WORK_DIR}/Empty" none)
expect_equal("${none}" "" "ce_collect_application_content: an empty folder must give no files")

file(REMOVE_RECURSE "${WORK_DIR}")
message(STATUS "ApplicationContentTests: passed")
