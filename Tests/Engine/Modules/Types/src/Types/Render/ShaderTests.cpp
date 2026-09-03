//
// Module: CelestialEngine/Tests/Engine/Modules/Types/Render
// File: ShaderTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-13
// Updated by: Catalin Chirosca
// Updated: 2026-09-03
//

#include <Types/Render/Shader.hpp>

#include <Types/Render/Render.hpp>

#include <glad/glad.h>
#include <gtest/gtest.h>

#include <format>

using namespace CE::Types;


namespace {

/**
 * @brief Test fixture for ShaderType tests
 */
class ShaderTypesTest: public ::testing::Test {};

}

// ============================================================================
// Naming Tests
// ============================================================================

/**
 * @brief Test that formatting a shader stage gives its name
 */
TEST_F(ShaderTypesTest, Format_NamesTheShaderStage) {
	EXPECT_EQ(std::format("{}", ShaderType::Vertex), "Vertex");
	EXPECT_EQ(std::format("{}", ShaderType::Fragment), "Fragment");
	EXPECT_EQ(std::format("{}", ShaderType::Compute), "Compute");
	EXPECT_EQ(std::format("{}", ShaderType::Geometry), "Geometry");
	EXPECT_EQ(std::format("{}", ShaderType::TessellationControl), "TessellationControl");
	EXPECT_EQ(std::format("{}", ShaderType::TessellationEvaluation), "TessellationEvaluation");
	EXPECT_EQ(std::format("{}", ShaderType::Mesh), "Mesh");
	EXPECT_EQ(std::format("{}", ShaderType::Amplification), "Amplification");
	EXPECT_EQ(std::format("{}", ShaderType::RayGeneration), "RayGeneration");
	EXPECT_EQ(std::format("{}", ShaderType::Unknown), "Unknown");
}

// ============================================================================
// API Support Tests
// ============================================================================

/**
 * @brief Test that the stages common to every API are supported
 */
TEST_F(ShaderTypesTest, IsShaderTypeSupported_CommonStages_AreSupported) {
	for (const auto api: {GraphicsApi::OpenGL, GraphicsApi::Metal, GraphicsApi::Vulkan, GraphicsApi::DirectX11,
			GraphicsApi::DirectX12}) {
		EXPECT_TRUE(IsShaderTypeSupported(api, ShaderType::Vertex));
		EXPECT_TRUE(IsShaderTypeSupported(api, ShaderType::Fragment));
		EXPECT_TRUE(IsShaderTypeSupported(api, ShaderType::Compute));
	}
}

/**
 * @brief Test that OpenGL supports the geometry and tessellation stages but not the mesh/ray-tracing ones
 */
TEST_F(ShaderTypesTest, IsShaderTypeSupported_OpenGl_HasNoMeshOrRayTracingStages) {
	EXPECT_TRUE(IsShaderTypeSupported(GraphicsApi::OpenGL, ShaderType::Geometry));
	EXPECT_TRUE(IsShaderTypeSupported(GraphicsApi::OpenGL, ShaderType::TessellationControl));
	EXPECT_TRUE(IsShaderTypeSupported(GraphicsApi::OpenGL, ShaderType::TessellationEvaluation));

	EXPECT_FALSE(IsShaderTypeSupported(GraphicsApi::OpenGL, ShaderType::Mesh));
	EXPECT_FALSE(IsShaderTypeSupported(GraphicsApi::OpenGL, ShaderType::Amplification));
	EXPECT_FALSE(IsShaderTypeSupported(GraphicsApi::OpenGL, ShaderType::RayGeneration));
}

/**
 * @brief Test that Metal supports the mesh/ray-tracing stages but not the geometry and tessellation ones
 */
TEST_F(ShaderTypesTest, IsShaderTypeSupported_Metal_HasNoGeometryOrTessellationStages) {
	EXPECT_TRUE(IsShaderTypeSupported(GraphicsApi::Metal, ShaderType::Mesh));
	EXPECT_TRUE(IsShaderTypeSupported(GraphicsApi::Metal, ShaderType::Amplification));
	EXPECT_TRUE(IsShaderTypeSupported(GraphicsApi::Metal, ShaderType::RayGeneration));

	EXPECT_FALSE(IsShaderTypeSupported(GraphicsApi::Metal, ShaderType::Geometry));
	EXPECT_FALSE(IsShaderTypeSupported(GraphicsApi::Metal, ShaderType::TessellationControl));
	EXPECT_FALSE(IsShaderTypeSupported(GraphicsApi::Metal, ShaderType::TessellationEvaluation));
}

/**
 * @brief Test that no stage is supported without a graphics API
 */
TEST_F(ShaderTypesTest, IsShaderTypeSupported_NoApi_SupportsNothing) {
	EXPECT_FALSE(IsShaderTypeSupported(GraphicsApi::None, ShaderType::Vertex));
	EXPECT_FALSE(IsShaderTypeSupported(GraphicsApi::None, ShaderType::Fragment));
}

/**
 * @brief Test that an unknown stage is never supported
 */
TEST_F(ShaderTypesTest, IsShaderTypeSupported_UnknownStage_IsNeverSupported) {
	EXPECT_FALSE(IsShaderTypeSupported(GraphicsApi::OpenGL, ShaderType::Unknown));
	EXPECT_FALSE(IsShaderTypeSupported(GraphicsApi::Metal, ShaderType::Unknown));
}

// ============================================================================
// OpenGL Mapping Tests
// ============================================================================

/**
 * @brief Test that the stages the OpenGL back-end compiles map onto their GL constants
 */
TEST_F(ShaderTypesTest, ToOpenGlShaderType_MapsCompilableStages) {
	EXPECT_EQ(ToOpenGlShaderType(ShaderType::Vertex), GL_VERTEX_SHADER);
	EXPECT_EQ(ToOpenGlShaderType(ShaderType::Fragment), GL_FRAGMENT_SHADER);
}

/**
 * @brief Test that a stage without a GL mapping is reported as invalid
 */
TEST_F(ShaderTypesTest, ToOpenGlShaderType_UnmappedStage_ReturnsInvalid) {
	EXPECT_EQ(ToOpenGlShaderType(ShaderType::Mesh), -1);
	EXPECT_EQ(ToOpenGlShaderType(ShaderType::Unknown), -1);
}
