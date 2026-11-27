//
// Module: CelestialEngine/Engine/Tests/Runtime/Core/CMD
// File: WindowCMDTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-27
//

#include <Core/CMD/Window.hpp>

#include <Define/Config.hpp>

#include <Tools/Tools.hpp>
#include <Types/Window/WindowProps.hpp>

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include <format>
#include <string>

using namespace CE::Core;
using namespace CE::Tools;
using namespace CE::Types;

namespace {

/**
 * @brief Test fixture for Window CMD tests
 */
class WindowCMDTest: public ::testing::Test {
protected:
	void SetUp() override {
		Log::Init();
	}

	void TearDown() override {
		Log::Shutdown();
	}
};

}

/**
 * @brief Test default window properties with no arguments
 * @details The APIs follow the build: the first entry of CE_RENDER_BACKENDS, and the window it pairs with.
 */
TEST_F(WindowCMDTest, ParseWindowArgs_NoArguments_ReturnsDefaults) {
	const char* argv[] = {"program"};
	constexpr int argc = 1;

	const WindowProps props = ParseWindowArgs(argc, argv);

	EXPECT_TRUE(props.title.empty());
	EXPECT_EQ(props.width, 1280);
	EXPECT_EQ(props.height, 720);
	EXPECT_TRUE(props.VSync);
	EXPECT_EQ(props.graphicsApi, GetDefaultGraphicsApi());
	EXPECT_EQ(props.windowApi, GetDefaultWindowApi(GetDefaultGraphicsApi()));
}

/**
 * @brief Test that the title is no longer an option
 * @details The title is the application's display name (InitFromCommandLine sets it). "--title" / "-t" and their value
 *			are reported as unsupported and skipped, and the options after them are still parsed.
 */
TEST_F(WindowCMDTest, ParseWindowArgs_TitleOption_IsIgnored) {
	const char* argv[] = {
		"program",
		"--title",
		"MyWindow",
		"-t",
		"GameEngine",
		"-w",
		"800"
	};
	constexpr int argc = 7;

	const WindowProps props = ParseWindowArgs(argc, argv);

	EXPECT_TRUE(props.title.empty());
	EXPECT_EQ(props.width, 800);
}

/**
 * @brief Test width argument with long form
 */
TEST_F(WindowCMDTest, ParseWindowArgs_WidthLongForm_SetsWidth) {
	const char* argv[] = {
		"program",
		"--width",
		"1920"
	};
	constexpr int argc = 3;

	const WindowProps props = ParseWindowArgs(argc, argv);

	EXPECT_EQ(props.width, 1920);
}

/**
 * @brief Test width argument with short form
 */
TEST_F(WindowCMDTest, ParseWindowArgs_WidthShortForm_SetsWidth) {
	const char* argv[] = {
		"program",
		"-w",
		"2560"
	};
	constexpr int argc = 3;

	const WindowProps props = ParseWindowArgs(argc, argv);

	EXPECT_EQ(props.width, 2560);
}

/**
 * @brief Test height argument with long form
 */
TEST_F(WindowCMDTest, ParseWindowArgs_HeightLongForm_SetsHeight) {
	const char* argv[] = {
		"program",
		"--height",
		"1080"
	};
	constexpr int argc = 3;

	const WindowProps props = ParseWindowArgs(argc, argv);

	EXPECT_EQ(props.height, 1080);
}

/**
 * @brief Test height argument with short form
 */
TEST_F(WindowCMDTest, ParseWindowArgs_HeightShortForm_SetsHeight) {
	const char* argv[] = {
		"program",
		"-h",
		"1440"
	};
	constexpr int argc = 3;

	const WindowProps props = ParseWindowArgs(argc, argv);

	EXPECT_EQ(props.height, 1440);
}

/**
 * @brief Test VSync true with long form
 */
TEST_F(WindowCMDTest, ParseWindowArgs_VSyncTrueLongForm_EnablesVSync) {
	const char* argv[] = {
		"program",
		"--vsync",
		"true"
	};
	constexpr int argc = 3;

	const WindowProps props = ParseWindowArgs(argc, argv);

	EXPECT_TRUE(props.VSync);
}

/**
 * @brief Test VSync false with long form
 */
TEST_F(WindowCMDTest, ParseWindowArgs_VSyncFalseLongForm_DisablesVSync) {
	const char* argv[] = {
		"program",
		"--vsync",
		"false"
	};
	constexpr int argc = 3;

	const WindowProps props = ParseWindowArgs(argc, argv);

	EXPECT_FALSE(props.VSync);
}

/**
 * @brief Test VSync with numeric value 1
 */
TEST_F(WindowCMDTest, ParseWindowArgs_VSyncOne_EnablesVSync) {
	const char* argv[] = {
		"program",
		"-v",
		"1"
	};
	constexpr int argc = 3;

	const WindowProps props = ParseWindowArgs(argc, argv);

	EXPECT_TRUE(props.VSync);
}

/**
 * @brief Test VSync with numeric value 0
 */
TEST_F(WindowCMDTest, ParseWindowArgs_VSyncZero_DisablesVSync) {
	const char* argv[] = {
		"program",
		"-v",
		"0"
	};
	constexpr int argc = 3;

	const WindowProps props = ParseWindowArgs(argc, argv);

	EXPECT_FALSE(props.VSync);
}

/**
 * @brief Test graphics API OpenGL
 */
TEST_F(WindowCMDTest, ParseWindowArgs_GraphicsApiOpenGL_SetsOpenGL) {
	const char* argv[] = {
		"program",
		"--graphics-api",
		"opengl"
	};
	constexpr int argc = 3;

	const WindowProps props = ParseWindowArgs(argc, argv);

	EXPECT_EQ(props.graphicsApi, GraphicsApi::OpenGL);
}

/**
 * @brief Test graphics API Metal
 */
TEST_F(WindowCMDTest, ParseWindowArgs_GraphicsApiMetal_SetsMetal) {
	const char* argv[] = {
		"program",
		"-g",
		"metal"
	};
	constexpr int argc = 3;

	const WindowProps props = ParseWindowArgs(argc, argv);

	EXPECT_EQ(props.graphicsApi, GraphicsApi::Metal);
}

/**
 * @brief Test graphics API Vulkan
 */
TEST_F(WindowCMDTest, ParseWindowArgs_GraphicsApiVulkan_SetsVulkan) {
	const char* argv[] = {
		"program",
		"-g",
		"vulkan"
	};
	constexpr int argc = 3;

	const WindowProps props = ParseWindowArgs(argc, argv);

	EXPECT_EQ(props.graphicsApi, GraphicsApi::Vulkan);
}

/**
 * @brief Test graphics API DirectX11 with full name
 */
TEST_F(WindowCMDTest, ParseWindowArgs_GraphicsApiDirectX11_SetsDirectX11) {
	const char* argv[] = {
		"program",
		"-g",
		"directx11"
	};
	constexpr int argc = 3;

	const WindowProps props = ParseWindowArgs(argc, argv);

	EXPECT_EQ(props.graphicsApi, GraphicsApi::DirectX11);
}

/**
 * @brief Test graphics API DirectX11 with dx11 alias
 */
TEST_F(WindowCMDTest, ParseWindowArgs_GraphicsApiDX11_SetsDirectX11) {
	const char* argv[] = {
		"program",
		"-g",
		"dx11"
	};
	constexpr int argc = 3;

	const WindowProps props = ParseWindowArgs(argc, argv);

	EXPECT_EQ(props.graphicsApi, GraphicsApi::DirectX11);
}

/**
 * @brief Test graphics API DirectX11 with d3d11 alias
 */
TEST_F(WindowCMDTest, ParseWindowArgs_GraphicsApiD3D11_SetsDirectX11) {
	const char* argv[] = {
		"program",
		"-g",
		"d3d11"
	};
	constexpr int argc = 3;

	const WindowProps props = ParseWindowArgs(argc, argv);

	EXPECT_EQ(props.graphicsApi, GraphicsApi::DirectX11);
}

/**
 * @brief Test graphics API DirectX12
 */
TEST_F(WindowCMDTest, ParseWindowArgs_GraphicsApiDirectX12_SetsDirectX12) {
	const char* argv[] = {
		"program",
		"-g",
		"directx12"
	};
	constexpr int argc = 3;

	const WindowProps props = ParseWindowArgs(argc, argv);

	EXPECT_EQ(props.graphicsApi, GraphicsApi::DirectX12);
}

/**
 * @brief Test graphics API with unsupported value defaults to the build's default API
 */
TEST_F(WindowCMDTest, ParseWindowArgs_GraphicsApiUnsupported_DefaultsToTheBuildDefault) {
	const char* argv[] = {
		"program",
		"-g",
		"unsupported"
	};
	constexpr int argc = 3;

	const WindowProps props = ParseWindowArgs(argc, argv);

	EXPECT_EQ(props.graphicsApi, GetDefaultGraphicsApi());
}

/**
 * @brief Test window API GLFW with long form
 */
TEST_F(WindowCMDTest, ParseWindowArgs_WindowApiGLFWLongForm_SetsGLFW) {
	const char* argv[] = {
		"program",
		"--window-api",
		"glfw"
	};
	constexpr int argc = 3;

	const WindowProps props = ParseWindowArgs(argc, argv);

	EXPECT_EQ(props.windowApi, WindowApi::GLFW);
}

/**
 * @brief Test window API GLFW with short form
 */
TEST_F(WindowCMDTest, ParseWindowArgs_WindowApiGLFWShortForm_SetsGLFW) {
	const char* argv[] = {
		"program",
		"-wa",
		"glfw"
	};
	constexpr int argc = 3;

	const WindowProps props = ParseWindowArgs(argc, argv);

	EXPECT_EQ(props.windowApi, WindowApi::GLFW);
}

/**
 * @brief Test window API Win32
 */
TEST_F(WindowCMDTest, ParseWindowArgs_WindowApiWin32_SetsWin32) {
	const char* argv[] = {
		"program",
		"--window-api",
		"win32"
	};
	constexpr int argc = 3;

	const WindowProps props = ParseWindowArgs(argc, argv);

	EXPECT_EQ(props.windowApi, WindowApi::Win32);
}

/**
 * @brief Test window API X11
 */
TEST_F(WindowCMDTest, ParseWindowArgs_WindowApiX11_SetsX11) {
	const char* argv[] = {
		"program",
		"-wa",
		"x11"
	};
	constexpr int argc = 3;

	const WindowProps props = ParseWindowArgs(argc, argv);

	EXPECT_EQ(props.windowApi, WindowApi::X11);
}

/**
 * @brief Test window API Cocoa
 */
TEST_F(WindowCMDTest, ParseWindowArgs_WindowApiCocoa_SetsCocoa) {
	const char* argv[] = {
		"program",
		"-wa",
		"cocoa"
	};
	constexpr int argc = 3;

	const WindowProps props = ParseWindowArgs(argc, argv);

	EXPECT_EQ(props.windowApi, WindowApi::Cocoa);
}

/**
 * @brief Test window API with unsupported value defaults to the window the graphics API pairs with
 */
TEST_F(WindowCMDTest, ParseWindowArgs_WindowApiUnsupported_DefaultsToTheGraphicsApisWindow) {
	const char* argv[] = {
		"program",
		"-wa",
		"unsupported"
	};
	constexpr int argc = 3;

	const WindowProps props = ParseWindowArgs(argc, argv);

	EXPECT_EQ(props.windowApi, GetDefaultWindowApi(props.graphicsApi));
}

/**
 * @brief Test that without -wa the window follows the graphics API asked for, wherever -g appears
 */
TEST_F(WindowCMDTest, ParseWindowArgs_NoWindowApi_FollowsTheGraphicsApi) {
	const char* metal[] = {"program", "-g", "metal"};
	const char* opengl[] = {"program", "-g", "opengl"};

	EXPECT_EQ(ParseWindowArgs(3, metal).windowApi, WindowApi::Cocoa);
	EXPECT_EQ(ParseWindowArgs(3, opengl).windowApi, WindowApi::GLFW);
}

/**
 * @brief Test that an explicit -wa wins over the pairing, even when it comes before -g
 */
TEST_F(WindowCMDTest, ParseWindowArgs_ExplicitWindowApi_WinsOverThePairing) {
	const char* argv[] = {"program", "-wa", "glfw", "-g", "metal"};

	const WindowProps props = ParseWindowArgs(5, argv);

	EXPECT_EQ(props.graphicsApi, GraphicsApi::Metal);
	EXPECT_EQ(props.windowApi, WindowApi::GLFW);
}

/**
 * @brief Test window API case insensitivity
 */
TEST_F(WindowCMDTest, ParseWindowArgs_WindowApiUppercase_ParsesCorrectly) {
	const char* argv[] = {
		"program",
		"--window-api",
		"GLFW"
	};
	constexpr int argc = 3;

	const WindowProps props = ParseWindowArgs(argc, argv);

	EXPECT_EQ(props.windowApi, WindowApi::GLFW);
}

/**
 * @brief Test multiple arguments combined
 */
TEST_F(WindowCMDTest, ParseWindowArgs_MultipleArguments_SetsAllProperties) {
	const char* argv[] = {
		"program",
		"-w",
		"1920",
		"-h",
		"1080",
		"--vsync",
		"false",
		"-g",
		"vulkan",
		"-wa",
		"cocoa"
	};
	constexpr int argc = 11;

	const WindowProps props = ParseWindowArgs(argc, argv);

	EXPECT_EQ(props.width, 1920);
	EXPECT_EQ(props.height, 1080);
	EXPECT_FALSE(props.VSync);
	EXPECT_EQ(props.graphicsApi, GraphicsApi::Vulkan);
	EXPECT_EQ(props.windowApi, WindowApi::Cocoa);
}

/**
 * @brief Test case insensitivity for arguments
 */
TEST_F(WindowCMDTest, ParseWindowArgs_UppercaseArguments_ParsesCorrectly) {
	const char* argv[] = {
		"program",
		"--HEIGHT",
		"600",
		"--WIDTH",
		"800"
	};
	constexpr int argc = 5;

	const WindowProps props = ParseWindowArgs(argc, argv);

	EXPECT_EQ(props.height, 600);
	EXPECT_EQ(props.width, 800);
}

/**
 * @brief Test invalid width throws exception
 */
TEST_F(WindowCMDTest, ParseWindowArgs_InvalidWidth_ThrowsException) {
	const char* argv[] = {
		"program",
		"--width",
		"invalid"
	};
	constexpr int argc = 3;

	EXPECT_THROW((void)ParseWindowArgs(argc, argv), std::runtime_error);
}

/**
 * @brief Test invalid height throws exception
 */
TEST_F(WindowCMDTest, ParseWindowArgs_InvalidHeight_ThrowsException) {
	const char* argv[] = {
		"program",
		"--height",
		"notanumber"
	};
	constexpr int argc = 3;

	EXPECT_THROW((void)ParseWindowArgs(argc, argv), std::runtime_error);
}

/**
 * @brief Test width out of range throws exception
 */
TEST_F(WindowCMDTest, ParseWindowArgs_WidthOutOfRange_ThrowsException) {
	const char* argv[] = {
		"program",
		"--width",
		"99999999999999999999"
	};
	constexpr int argc = 3;

	EXPECT_THROW((void)ParseWindowArgs(argc, argv), std::runtime_error);
}

/**
 * @brief Test missing value for argument uses defaults
 */
TEST_F(WindowCMDTest, ParseWindowArgs_MissingValue_UsesDefaults) {
	const char* argv[] = {
		"program",
		"--width"
	};
	constexpr int argc = 2;

	const WindowProps props = ParseWindowArgs(argc, argv);

	EXPECT_EQ(props.width, 1280); // Should use default
}

/**
 * @brief Test unrecognized arguments are ignored
 */
TEST_F(WindowCMDTest, ParseWindowArgs_UnrecognizedArguments_Ignored) {
	const char* argv[] = {
		"program",
		"--unknown",
		"value",
		"--width",
		"1024"
	};
	constexpr int argc = 5;

	const WindowProps props = ParseWindowArgs(argc, argv);

	EXPECT_EQ(props.width, 1024);
}

// ============================================================================
// Unsupported Option Value Tests
// ============================================================================

namespace {

/// What ParseWindowArgs returned, and what the loggers wrote while it ran.
struct ParsedWithLog {
	WindowProps props;
	std::string log;
};

/**
 * @brief Runs ParseWindowArgs capturing stdout, where the Debug/Release loggers write
 * @details Captured at the file-descriptor level (gtest), not with an extra spdlog sink: a sink built in this executable
 *			instantiates spdlog's templates here too, and the second copy of its registry makes the next Log::Init throw
 *			"logger with name 'CoreLogger' already exists".
 */
ParsedWithLog ParseCapturingLog(const int argc, const char* argv[]) {
	::testing::internal::CaptureStdout();
	auto props = ParseWindowArgs(argc, argv);
	Log::Flush();
	return {std::move(props), ::testing::internal::GetCapturedStdout()};
}

/// How many "is not supported" warnings the log holds.
int UnsupportedWarnings(const std::string_view log) {
	int count = 0;
	for (auto at = log.find("is not supported"); at != std::string_view::npos; at = log.find("is not supported", at + 1))
		++count;
	return count;
}

/// Whether a warning named this argument as the unsupported one.
bool Reported(const std::string_view log, const std::string_view argument) {
	return log.contains(std::format("({}) is not supported", argument));
}

}

/**
 * @brief Test that the value of an unsupported option is skipped with it
 * @details One warning, for the option: its value is not reported as a second unsupported parameter. The warning is
 *			compiled out in Dist, so there only the parsing after it is checked.
 */
TEST_F(WindowCMDTest, ParseWindowArgs_UnsupportedOption_SkipsItsValue) {
	const char* argv[] = {"program", "--unknown", "value", "-w", "1024"};

	const auto [props, log] = ParseCapturingLog(5, argv);

	EXPECT_EQ(props.width, 1024);
#if not CE_DIST
	EXPECT_EQ(UnsupportedWarnings(log), 1);
	EXPECT_TRUE(Reported(log, "--unknown"));
	EXPECT_FALSE(Reported(log, "value"));
#endif
}

/**
 * @brief Test that an option right after an unsupported one is not taken for its value
 * @details "-w" starts with '-', so it is parsed as an option: the width is set, whatever the configuration.
 */
TEST_F(WindowCMDTest, ParseWindowArgs_UnsupportedOptionFollowedByAnOption_DoesNotSkipIt) {
	const char* argv[] = {"program", "--unknown", "-w", "1024"};

	const auto [props, log] = ParseCapturingLog(4, argv);

	EXPECT_EQ(props.width, 1024);
#if not CE_DIST
	EXPECT_EQ(UnsupportedWarnings(log), 1);
#endif
}

/**
 * @brief Test that two unsupported options in a row are both reported, neither taken for the other's value
 */
TEST_F(WindowCMDTest, ParseWindowArgs_TwoUnsupportedOptions_ReportsBoth) {
	const char* argv[] = {"program", "--first", "--second", "value", "-h", "600"};

	const auto [props, log] = ParseCapturingLog(6, argv);

	EXPECT_EQ(props.height, 600);
#if not CE_DIST
	EXPECT_EQ(UnsupportedWarnings(log), 2);
	EXPECT_TRUE(Reported(log, "--first"));
	EXPECT_TRUE(Reported(log, "--second"));
	EXPECT_FALSE(Reported(log, "value"));
#endif
}

/**
 * @brief Test that an unsupported option in last position has nothing to skip and does not read past argv
 */
TEST_F(WindowCMDTest, ParseWindowArgs_UnsupportedOptionInLastPosition_IsReportedAlone) {
	const char* argv[] = {"program", "-w", "800", "--unknown"};

	const auto [props, log] = ParseCapturingLog(4, argv);

	EXPECT_EQ(props.width, 800);
#if not CE_DIST
	EXPECT_EQ(UnsupportedWarnings(log), 1);
	EXPECT_TRUE(Reported(log, "--unknown"));
#endif
}

/**
 * @brief Test that a supported option without its value (last position) is reported and leaves the default
 * @details It takes the unsupported path too: nothing follows it, so nothing is skipped.
 */
TEST_F(WindowCMDTest, ParseWindowArgs_SupportedOptionWithoutValue_IsReportedAsUnsupported) {
	const char* argv[] = {"program", "-h", "600", "-w"};

	const auto [props, log] = ParseCapturingLog(4, argv);

	EXPECT_EQ(props.height, 600);
	EXPECT_EQ(props.width, 1280);
#if not CE_DIST
	EXPECT_EQ(UnsupportedWarnings(log), 1);
	EXPECT_TRUE(Reported(log, "-w"));
#endif
}

// ============================================================================
// Refresh Rate Tests
// ============================================================================

/**
 * @brief Test that the refresh rate defaults to 0 (uncapped)
 */
TEST_F(WindowCMDTest, ParseWindowArgs_NoRefreshArgument_DefaultsToUncapped) {
	const char* argv[] = {"program"};
	constexpr int argc = 1;

	const WindowProps props = ParseWindowArgs(argc, argv);

	EXPECT_EQ(props.refreshRate, 0);
}

/**
 * @brief Test the refresh rate argument in long form
 */
TEST_F(WindowCMDTest, ParseWindowArgs_RefreshLongForm_SetsRefreshRate) {
	const char* argv[] = {
		"program",
		"--refresh",
		"144"
	};
	constexpr int argc = 3;

	const WindowProps props = ParseWindowArgs(argc, argv);

	EXPECT_EQ(props.refreshRate, 144);
}

/**
 * @brief Test the refresh rate argument in short form
 */
TEST_F(WindowCMDTest, ParseWindowArgs_RefreshShortForm_SetsRefreshRate) {
	const char* argv[] = {
		"program",
		"-r",
		"60"
	};
	constexpr int argc = 3;

	const WindowProps props = ParseWindowArgs(argc, argv);

	EXPECT_EQ(props.refreshRate, 60);
}

/**
 * @brief Test that an invalid refresh rate throws
 */
TEST_F(WindowCMDTest, ParseWindowArgs_InvalidRefresh_ThrowsException) {
	const char* argv[] = {
		"program",
		"--refresh",
		"fast"
	};
	constexpr int argc = 3;

	EXPECT_THROW((void)ParseWindowArgs(argc, argv), std::runtime_error);
}

// ============================================================================
// Validation Tests
// ============================================================================

namespace {

/// Properties for the given pair, everything else at its usual value.
WindowProps PropsFor(const GraphicsApi graphicsApi, const WindowApi windowApi) {
	return {"Test", 1280, 720, true, 0, graphicsApi, windowApi};
}

}

/**
 * @brief Test that the build's own defaults always pass validation
 * @details What a launch with no arguments depends on, whatever CE_RENDER_BACKENDS selected.
 */
TEST_F(WindowCMDTest, ValidateWindowProps_BuildDefaults_Pass) {
	const char* argv[] = {"program"};

	EXPECT_NO_THROW(ValidateWindowProps(ParseWindowArgs(1, argv)));
}

/**
 * @brief Test that every compiled-in graphics API passes with the window it pairs with
 */
TEST_F(WindowCMDTest, ValidateWindowProps_EveryCompiledApiWithItsWindow_Passes) {
	for (const auto api: {GraphicsApi::OpenGL, GraphicsApi::Metal, GraphicsApi::Vulkan, GraphicsApi::DirectX11, GraphicsApi::DirectX12}) {
		if (IsGraphicsApiCompiled(api))
			EXPECT_NO_THROW(ValidateWindowProps(PropsFor(api, GetDefaultWindowApi(api)))) << format_as(api);
	}
}

/**
 * @brief Test that a graphics API this build lacks is rejected
 * @details Vulkan has no backend yet, so it is missing from every build.
 */
TEST_F(WindowCMDTest, ValidateWindowProps_GraphicsApiNotCompiled_Throws) {
	EXPECT_THROW(ValidateWindowProps(PropsFor(GraphicsApi::Vulkan, WindowApi::GLFW)), std::runtime_error);
}

/**
 * @brief Test that a window API this build lacks is rejected
 * @details Win32 has no backend yet, so it is missing from every build.
 */
TEST_F(WindowCMDTest, ValidateWindowProps_WindowApiNotCompiled_Throws) {
	EXPECT_THROW(ValidateWindowProps(PropsFor(GetDefaultGraphicsApi(), WindowApi::Win32)), std::runtime_error);
}

#if CE_RENDER_METAL and CE_WINDOW_GLFW
/**
 * @brief Test that two compiled-in APIs that cannot work together are rejected
 * @details Needs both Metal and GLFW in the build to be meaningful - otherwise one of the earlier checks fires first.
 */
TEST_F(WindowCMDTest, ValidateWindowProps_IncompatiblePair_Throws) {
	EXPECT_THROW(ValidateWindowProps(PropsFor(GraphicsApi::Metal, WindowApi::GLFW)), std::runtime_error);
}
#endif
