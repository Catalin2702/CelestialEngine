//
// Module: CelestialEngine/Tests/Engine/Modules/Tools/CMD
// File: WindowCMDTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-02
// Updated by: Catalin Chirosca
// Updated: 2026-03-29
//

#include <Utility/CMD/Window.hpp>

#include <Tools/Log/Log.hpp>
#include <Types/Window/WindowProps.hpp>

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

using namespace CE::Utility::CMD;
using namespace CE::Tools::Log;
using namespace CE::Types::Render;
using namespace CE::Types::Window;

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

/**
 * @brief Test default window properties with no arguments
 */
TEST_F(WindowCMDTest, GetWindowProps_NoArguments_ReturnsDefaults) {
	const char* argv[] = {"program"};
	constexpr int argc = 1;

	const WindowProps props = GetWindowProps(argc, argv);

	EXPECT_EQ(props.title, "CelestialEngine");
	EXPECT_EQ(props.width, 1280);
	EXPECT_EQ(props.height, 720);
	EXPECT_TRUE(props.VSync);
	EXPECT_EQ(props.graphicsApi, GraphicsApi::OpenGL);
	EXPECT_EQ(props.windowApi, WindowApi::GLFW);
}

/**
 * @brief Test title argument with long form
 */
TEST_F(WindowCMDTest, GetWindowProps_TitleLongForm_SetsTitle) {
	const char* argv[] = {
		"program",
		"--title",
		"MyWindow"
	};
	constexpr int argc = 3;

	const WindowProps props = GetWindowProps(argc, argv);

	EXPECT_EQ(props.title, "MyWindow");
}

/**
 * @brief Test title argument with short form
 */
TEST_F(WindowCMDTest, GetWindowProps_TitleShortForm_SetsTitle) {
	const char* argv[] = {
		"program",
		"-t",
		"GameEngine"
	};
	constexpr int argc = 3;

	const WindowProps props = GetWindowProps(argc, argv);

	EXPECT_EQ(props.title, "GameEngine");
}

/**
 * @brief Test width argument with long form
 */
TEST_F(WindowCMDTest, GetWindowProps_WidthLongForm_SetsWidth) {
	const char* argv[] = {
		"program",
		"--width",
		"1920"
	};
	constexpr int argc = 3;

	const WindowProps props = GetWindowProps(argc, argv);

	EXPECT_EQ(props.width, 1920);
}

/**
 * @brief Test width argument with short form
 */
TEST_F(WindowCMDTest, GetWindowProps_WidthShortForm_SetsWidth) {
	const char* argv[] = {
		"program",
		"-w",
		"2560"
	};
	constexpr int argc = 3;

	const WindowProps props = GetWindowProps(argc, argv);

	EXPECT_EQ(props.width, 2560);
}

/**
 * @brief Test height argument with long form
 */
TEST_F(WindowCMDTest, GetWindowProps_HeightLongForm_SetsHeight) {
	const char* argv[] = {
		"program",
		"--height",
		"1080"
	};
	constexpr int argc = 3;

	const WindowProps props = GetWindowProps(argc, argv);

	EXPECT_EQ(props.height, 1080);
}

/**
 * @brief Test height argument with short form
 */
TEST_F(WindowCMDTest, GetWindowProps_HeightShortForm_SetsHeight) {
	const char* argv[] = {
		"program",
		"-h",
		"1440"
	};
	constexpr int argc = 3;

	const WindowProps props = GetWindowProps(argc, argv);

	EXPECT_EQ(props.height, 1440);
}

/**
 * @brief Test VSync true with long form
 */
TEST_F(WindowCMDTest, GetWindowProps_VSyncTrueLongForm_EnablesVSync) {
	const char* argv[] = {
		"program",
		"--vsync",
		"true"
	};
	constexpr int argc = 3;

	const WindowProps props = GetWindowProps(argc, argv);

	EXPECT_TRUE(props.VSync);
}

/**
 * @brief Test VSync false with long form
 */
TEST_F(WindowCMDTest, GetWindowProps_VSyncFalseLongForm_DisablesVSync) {
	const char* argv[] = {
		"program",
		"--vsync",
		"false"
	};
	constexpr int argc = 3;

	const WindowProps props = GetWindowProps(argc, argv);

	EXPECT_FALSE(props.VSync);
}

/**
 * @brief Test VSync with numeric value 1
 */
TEST_F(WindowCMDTest, GetWindowProps_VSyncOne_EnablesVSync) {
	const char* argv[] = {
		"program",
		"-v",
		"1"
	};
	constexpr int argc = 3;

	const WindowProps props = GetWindowProps(argc, argv);

	EXPECT_TRUE(props.VSync);
}

/**
 * @brief Test VSync with numeric value 0
 */
TEST_F(WindowCMDTest, GetWindowProps_VSyncZero_DisablesVSync) {
	const char* argv[] = {
		"program",
		"-v",
		"0"
	};
	constexpr int argc = 3;

	const WindowProps props = GetWindowProps(argc, argv);

	EXPECT_FALSE(props.VSync);
}

/**
 * @brief Test graphics API OpenGL
 */
TEST_F(WindowCMDTest, GetWindowProps_GraphicsApiOpenGL_SetsOpenGL) {
	const char* argv[] = {
		"program",
		"--graphics-api",
		"opengl"
	};
	constexpr int argc = 3;

	const WindowProps props = GetWindowProps(argc, argv);

	EXPECT_EQ(props.graphicsApi, GraphicsApi::OpenGL);
}

/**
 * @brief Test graphics API Metal
 */
TEST_F(WindowCMDTest, GetWindowProps_GraphicsApiMetal_SetsMetal) {
	const char* argv[] = {
		"program",
		"-g",
		"metal"
	};
	constexpr int argc = 3;

	const WindowProps props = GetWindowProps(argc, argv);

	EXPECT_EQ(props.graphicsApi, GraphicsApi::Metal);
}

/**
 * @brief Test graphics API Vulkan
 */
TEST_F(WindowCMDTest, GetWindowProps_GraphicsApiVulkan_SetsVulkan) {
	const char* argv[] = {
		"program",
		"-g",
		"vulkan"
	};
	constexpr int argc = 3;

	const WindowProps props = GetWindowProps(argc, argv);

	EXPECT_EQ(props.graphicsApi, GraphicsApi::Vulkan);
}

/**
 * @brief Test graphics API DirectX11 with full name
 */
TEST_F(WindowCMDTest, GetWindowProps_GraphicsApiDirectX11_SetsDirectX11) {
	const char* argv[] = {
		"program",
		"-g",
		"directx11"
	};
	constexpr int argc = 3;

	const WindowProps props = GetWindowProps(argc, argv);

	EXPECT_EQ(props.graphicsApi, GraphicsApi::DirectX11);
}

/**
 * @brief Test graphics API DirectX11 with dx11 alias
 */
TEST_F(WindowCMDTest, GetWindowProps_GraphicsApiDX11_SetsDirectX11) {
	const char* argv[] = {
		"program",
		"-g",
		"dx11"
	};
	constexpr int argc = 3;

	const WindowProps props = GetWindowProps(argc, argv);

	EXPECT_EQ(props.graphicsApi, GraphicsApi::DirectX11);
}

/**
 * @brief Test graphics API DirectX11 with d3d11 alias
 */
TEST_F(WindowCMDTest, GetWindowProps_GraphicsApiD3D11_SetsDirectX11) {
	const char* argv[] = {
		"program",
		"-g",
		"d3d11"
	};
	constexpr int argc = 3;

	const WindowProps props = GetWindowProps(argc, argv);

	EXPECT_EQ(props.graphicsApi, GraphicsApi::DirectX11);
}

/**
 * @brief Test graphics API DirectX12
 */
TEST_F(WindowCMDTest, GetWindowProps_GraphicsApiDirectX12_SetsDirectX12) {
	const char* argv[] = {
		"program",
		"-g",
		"directx12"
	};
	constexpr int argc = 3;

	const WindowProps props = GetWindowProps(argc, argv);

	EXPECT_EQ(props.graphicsApi, GraphicsApi::DirectX12);
}

/**
 * @brief Test graphics API with unsupported value defaults to OpenGL
 */
TEST_F(WindowCMDTest, GetWindowProps_GraphicsApiUnsupported_DefaultsToOpenGL) {
	const char* argv[] = {
		"program",
		"-g",
		"unsupported"
	};
	constexpr int argc = 3;

	const WindowProps props = GetWindowProps(argc, argv);

	EXPECT_EQ(props.graphicsApi, GraphicsApi::OpenGL);
}

/**
 * @brief Test window API GLFW with long form
 */
TEST_F(WindowCMDTest, GetWindowProps_WindowApiGLFWLongForm_SetsGLFW) {
	const char* argv[] = {
		"program",
		"--window-api",
		"glfw"
	};
	constexpr int argc = 3;

	const WindowProps props = GetWindowProps(argc, argv);

	EXPECT_EQ(props.windowApi, WindowApi::GLFW);
}

/**
 * @brief Test window API GLFW with short form
 */
TEST_F(WindowCMDTest, GetWindowProps_WindowApiGLFWShortForm_SetsGLFW) {
	const char* argv[] = {
		"program",
		"-wa",
		"glfw"
	};
	constexpr int argc = 3;

	const WindowProps props = GetWindowProps(argc, argv);

	EXPECT_EQ(props.windowApi, WindowApi::GLFW);
}

/**
 * @brief Test window API Win32
 */
TEST_F(WindowCMDTest, GetWindowProps_WindowApiWin32_SetsWin32) {
	const char* argv[] = {
		"program",
		"--window-api",
		"win32"
	};
	constexpr int argc = 3;

	const WindowProps props = GetWindowProps(argc, argv);

	EXPECT_EQ(props.windowApi, WindowApi::Win32);
}

/**
 * @brief Test window API X11
 */
TEST_F(WindowCMDTest, GetWindowProps_WindowApiX11_SetsX11) {
	const char* argv[] = {
		"program",
		"-wa",
		"x11"
	};
	constexpr int argc = 3;

	const WindowProps props = GetWindowProps(argc, argv);

	EXPECT_EQ(props.windowApi, WindowApi::X11);
}

/**
 * @brief Test window API Cocoa
 */
TEST_F(WindowCMDTest, GetWindowProps_WindowApiCocoa_SetsCocoa) {
	const char* argv[] = {
		"program",
		"-wa",
		"cocoa"
	};
	constexpr int argc = 3;

	const WindowProps props = GetWindowProps(argc, argv);

	EXPECT_EQ(props.windowApi, WindowApi::Cocoa);
}

/**
 * @brief Test window API with unsupported value defaults to GLFW
 */
TEST_F(WindowCMDTest, GetWindowProps_WindowApiUnsupported_DefaultsToGLFW) {
	const char* argv[] = {
		"program",
		"-wa",
		"unsupported"
	};
	constexpr int argc = 3;

	const WindowProps props = GetWindowProps(argc, argv);

	EXPECT_EQ(props.windowApi, WindowApi::GLFW);
}

/**
 * @brief Test window API case insensitivity
 */
TEST_F(WindowCMDTest, GetWindowProps_WindowApiUppercase_ParsesCorrectly) {
	const char* argv[] = {
		"program",
		"--window-api",
		"GLFW"
	};
	constexpr int argc = 3;

	const WindowProps props = GetWindowProps(argc, argv);

	EXPECT_EQ(props.windowApi, WindowApi::GLFW);
}

/**
 * @brief Test multiple arguments combined
 */
TEST_F(WindowCMDTest, GetWindowProps_MultipleArguments_SetsAllProperties) {
	const char* argv[] = {
		"program",
		"--title",
		"TestWindow",
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
	constexpr int argc = 13;

	const WindowProps props = GetWindowProps(argc, argv);

	EXPECT_EQ(props.title, "TestWindow");
	EXPECT_EQ(props.width, 1920);
	EXPECT_EQ(props.height, 1080);
	EXPECT_FALSE(props.VSync);
	EXPECT_EQ(props.graphicsApi, GraphicsApi::Vulkan);
	EXPECT_EQ(props.windowApi, WindowApi::Cocoa);
}

/**
 * @brief Test case insensitivity for arguments
 */
TEST_F(WindowCMDTest, GetWindowProps_UppercaseArguments_ParsesCorrectly) {
	const char* argv[] = {
		"program",
		"--TITLE",
		"UpperCase",
		"--WIDTH",
		"800"
	};
	constexpr int argc = 5;

	const WindowProps props = GetWindowProps(argc, argv);

	EXPECT_EQ(props.title, "UpperCase");
	EXPECT_EQ(props.width, 800);
}

/**
 * @brief Test invalid width throws exception
 */
TEST_F(WindowCMDTest, GetWindowProps_InvalidWidth_ThrowsException) {
	const char* argv[] = {
		"program",
		"--width",
		"invalid"
	};
	constexpr int argc = 3;

	EXPECT_THROW(GetWindowProps(argc, argv), std::runtime_error);
}

/**
 * @brief Test invalid height throws exception
 */
TEST_F(WindowCMDTest, GetWindowProps_InvalidHeight_ThrowsException) {
	const char* argv[] = {
		"program",
		"--height",
		"notanumber"
	};
	constexpr int argc = 3;

	EXPECT_THROW(GetWindowProps(argc, argv), std::runtime_error);
}

/**
 * @brief Test width out of range throws exception
 */
TEST_F(WindowCMDTest, GetWindowProps_WidthOutOfRange_ThrowsException) {
	const char* argv[] = {
		"program",
		"--width",
		"99999999999999999999"
	};
	constexpr int argc = 3;

	EXPECT_THROW(GetWindowProps(argc, argv), std::runtime_error);
}

/**
 * @brief Test missing value for argument uses defaults
 */
TEST_F(WindowCMDTest, GetWindowProps_MissingValue_UsesDefaults) {
	const char* argv[] = {
		"program",
		"--width"
	};
	constexpr int argc = 2;

	const WindowProps props = GetWindowProps(argc, argv);

	EXPECT_EQ(props.width, 1280); // Should use default
}

/**
 * @brief Test unrecognized arguments are ignored
 */
TEST_F(WindowCMDTest, GetWindowProps_UnrecognizedArguments_Ignored) {
	const char* argv[] = {
		"program",
		"--unknown",
		"value",
		"--width",
		"1024"
	};
	constexpr int argc = 5;

	const WindowProps props = GetWindowProps(argc, argv);

	EXPECT_EQ(props.width, 1024);
}
