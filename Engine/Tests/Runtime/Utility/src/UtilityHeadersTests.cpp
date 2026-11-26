//
// Module: CelestialEngine/Engine/Tests/Runtime/Utility
// File: UtilityHeadersTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

// The Utility umbrella has to compile with nothing included before it.
#include <Utility/Utility.hpp>

#include <gtest/gtest.h>

#include <type_traits>

/**
 * @brief Test that the Utility umbrella exposes the module's main types
 */
TEST(UtilityHeadersTest, Umbrella_ExposesTheModule) {
	EXPECT_TRUE(std::is_class_v<CE::Utility::Chronometer>);
	EXPECT_TRUE(std::is_class_v<CE::Utility::Delegate<void()>>);
}

/**
 * @brief Test that Utility stays generic: none of the application-only pieces that moved to Core is reachable
 * @details CMD, Config and the ImGui glue used to live here, and brought ImGui (and GLFW) into every module that linked
 *			Utility - the RHI and CE_Apple included. The include guards are how the umbrella would let them back in.
 */
TEST(UtilityHeadersTest, Umbrella_PullsInNoApplicationCode) {
#ifdef CE_CORE_CONFIG_CONFIG_HPP
	FAIL() << "Utility/Utility.hpp pulled in Core/Config/Config.hpp";
#endif
#ifdef CE_CORE_LAYERS_IMGUI_IMGUIHELPERS_HPP
	FAIL() << "Utility/Utility.hpp pulled in the ImGui helpers";
#endif
#ifdef IMGUI_VERSION
	FAIL() << "Utility/Utility.hpp pulled in imgui.h";
#endif
	SUCCEED();
}
