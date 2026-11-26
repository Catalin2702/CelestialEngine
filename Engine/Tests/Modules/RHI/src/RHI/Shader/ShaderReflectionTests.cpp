//
// Module: CelestialEngine/Engine/Tests/Modules/RHI/Shader
// File: ShaderReflectionTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-26
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

#include <Types/Render/ShaderConstants.hpp>

#include <gtest/gtest.h>

#include <array>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <map>
#include <optional>
#include <ranges>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

// Engine/Shaders/<build>/Generated, set by this test's CMakeLists.txt from what Engine/Shaders exports.
#ifndef CE_SHADERS_GENERATED_DIR
	#error "CE_SHADERS_GENERATED_DIR is not defined - it is set by Engine/Tests/Modules/RHI/CMakeLists.txt."
#endif

namespace {

namespace fs = std::filesystem;

/**
 * @brief One line of a .reflect file: its kind and its key=value pairs
 * @details "uniform_buffer name=CameraConstants set=0 binding=1 size=64" is kind "uniform_buffer" and four values. A
 *			word with no '=' ("stage Vertex") is stored under the empty key.
 */
struct Entry {
	std::string kind;
	std::map<std::string, std::string, std::less<>> values;

	[[nodiscard]] std::string Get(const std::string_view key) const {
		const auto it = values.find(key);
		return it == values.end() ? std::string{} : it->second;
	}

	[[nodiscard]] unsigned Number(const std::string_view key) const { return static_cast<unsigned>(std::stoul(Get(key))); }
};

fs::path GeneratedDirectory() {
	return {CE_SHADERS_GENERATED_DIR};
}

std::string ReadFile(const fs::path& path) {
	std::ifstream file(path, std::ios::binary);
	return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

std::vector<Entry> ReadReflection(const std::string_view shaderName) {
	std::istringstream lines(ReadFile(GeneratedDirectory() / "Reflection" / (std::string(shaderName) + ".reflect")));

	std::vector<Entry> entries;
	std::string line;
	while (std::getline(lines, line)) {
		std::istringstream words(line);
		Entry entry;
		words >> entry.kind;

		std::string word;
		while (words >> word) {
			const auto equals = word.find('=');
			if (equals == std::string::npos)
				entry.values[""] = word;
			else
				entry.values[word.substr(0, equals)] = word.substr(equals + 1);
		}
		entries.push_back(std::move(entry));
	}
	return entries;
}

std::vector<Entry> All(const std::vector<Entry>& entries, const std::string_view kind) {
	std::vector<Entry> found;
	for (const auto& entry: entries) {
		if (entry.kind == kind)
			found.push_back(entry);
	}
	return found;
}

std::optional<Entry> Named(const std::vector<Entry>& entries, const std::string_view kind, const std::string_view name) {
	for (const auto& entry: entries) {
		if (entry.kind == kind and entry.Get("name") == name)
			return entry;
	}
	return std::nullopt;
}

std::optional<Entry> AtLocation(const std::vector<Entry>& entries, const std::string_view kind, const unsigned location) {
	for (const auto& entry: entries) {
		if (entry.kind == kind and entry.Number("location") == location)
			return entry;
	}
	return std::nullopt;
}

/// Every shader Engine/Shaders compiles, with its stage. constexpr: nothing to construct, so nothing can throw before main.
constexpr std::array<std::pair<std::string_view, std::string_view>, 4> Shaders{{
	{"CompositeFragment", "Fragment"},
	{"CompositeVertex", "Vertex"},
	{"Fragment", "Fragment"},
	{"Vertex", "Vertex"},
}};

}

// ============================================================================
// The reflection exists and says what it describes
// ============================================================================

TEST(ShaderReflectionTest, EveryShader_HasReflectionOfItsStage) {
	for (const auto& [name, stage]: Shaders) {
		const auto entries = ReadReflection(name);
		ASSERT_FALSE(entries.empty()) << name << ".reflect is missing or empty";
		EXPECT_EQ(entries.front().kind, "stage") << name;
		EXPECT_EQ(entries.front().Get(""), stage) << name;
	}
}

// ============================================================================
// The shaders agree with the C++ side
// ============================================================================

/**
 * @brief The constant blocks sit at the slots the renderer uploads them to, with the size of the C++ structs
 * @details ForwardRenderer calls SetVertexConstants(Types::CAMERA_CONSTANTS_SLOT, ...); the shader's binding is that
 *			slot in Metal ([[buffer(n)]]). A wrong binding draws nothing, or draws with the other block's matrix.
 */
TEST(ShaderReflectionTest, Vertex_ConstantBlocks_MatchEngineSlotsAndSizes) {
	const auto entries = ReadReflection("Vertex");

	const auto camera = Named(entries, "uniform_buffer", "CameraConstants");
	ASSERT_TRUE(camera.has_value());
	EXPECT_EQ(camera->Number("set"), 0u);
	EXPECT_EQ(camera->Number("binding"), CE::Types::CAMERA_CONSTANTS_SLOT);
	EXPECT_EQ(camera->Number("size"), sizeof(CE::Types::CameraConstants));

	const auto object = Named(entries, "uniform_buffer", "ObjectConstants");
	ASSERT_TRUE(object.has_value());
	EXPECT_EQ(object->Number("set"), 0u);
	EXPECT_EQ(object->Number("binding"), CE::Types::OBJECT_CONSTANTS_SLOT);
	EXPECT_EQ(object->Number("size"), sizeof(CE::Types::ObjectConstants));
}

/**
 * @brief Every constant block is one OpenGL can bind, under a slot every backend reserves
 * @details GLSL 4.10 has no binding qualifier: OpenGlPipelineState binds the blocks by these two names. A block with
 *			another name would compile and stay unbound in OpenGL only.
 */
TEST(ShaderReflectionTest, EveryConstantBlock_IsOneOpenGlBindsByName) {
	for (const auto& name: Shaders | std::views::keys) {
		for (const auto& block: All(ReadReflection(name), "uniform_buffer")) {
			const auto blockName = block.Get("name");
			EXPECT_TRUE(blockName == "CameraConstants" or blockName == "ObjectConstants") << name << ": " << blockName;
			EXPECT_LT(block.Number("binding"), CE::Types::MAX_CONSTANT_SLOTS) << name << ": " << blockName;
		}
	}
}

/**
 * @brief The scene vertex shader reads the vertex layout the engine uploads: position then colour
 * @details BufferLayout element i is attribute i: {Float3 position, Float4 colour} in SandboxLayer and in the composite
 *			quad. An input at another location, or of another size, reads the wrong bytes of every vertex.
 */
TEST(ShaderReflectionTest, VertexShaders_ReadPositionThenColour) {
	for (const auto* name: {"Vertex", "CompositeVertex"}) {
		const auto entries = ReadReflection(name);

		const auto position = AtLocation(entries, "input", 0);
		ASSERT_TRUE(position.has_value()) << name;
		EXPECT_EQ(position->Get("type"), "float3") << name;

		const auto colour = AtLocation(entries, "input", 1);
		ASSERT_TRUE(colour.has_value()) << name;
		EXPECT_EQ(colour->Get("type"), "float4") << name;
	}
}

/**
 * @brief What Fragment reads is what Vertex writes, location by location
 * @details The two are compiled separately; nothing but this check links the vertex output to the fragment input.
 */
TEST(ShaderReflectionTest, Fragment_Inputs_MatchVertexOutputs) {
	const auto vertex = ReadReflection("Vertex");
	const auto fragment = ReadReflection("Fragment");

	for (const auto& input: All(fragment, "input")) {
		const auto output = AtLocation(vertex, "output", input.Number("location"));
		ASSERT_TRUE(output.has_value()) << "no vertex output at location " << input.Get("location");
		EXPECT_EQ(output->Get("type"), input.Get("type")) << "location " << input.Get("location");
	}
}

/**
 * @brief The composite pass reads the scene texture at slot 0, under the name OpenGL binds it by
 * @details ForwardRenderer::Composite calls SetFragmentTexture(0, ...); OpenGlCommandEncoder looks the sampler up as
 *			"sceneColor".
 */
TEST(ShaderReflectionTest, CompositeFragment_ReadsSceneColorAtSlotZero) {
	const auto texture = Named(ReadReflection("CompositeFragment"), "sampled_image", "sceneColor");
	ASSERT_TRUE(texture.has_value());
	EXPECT_EQ(texture->Number("set"), 0u);
	EXPECT_EQ(texture->Number("binding"), 0u);
}

// ============================================================================
// The generated code is what each backend expects
// ============================================================================

#if CE_RENDER_OPENGL
/**
 * @brief The GLSL is 4.10, binds nothing by number and keeps OpenGL's own pixel origin
 * @details macOS stops at OpenGL 4.1: "binding =" needs 4.20 and would not compile there. origin_upper_left would turn
 *			gl_FragCoord upside down in the composite pass.
 */
TEST(ShaderReflectionTest, GeneratedGlsl_IsVersion410WithoutBindingsOrUpperLeftOrigin) {
	for (const auto& name: Shaders | std::views::keys) {
		const auto glsl = ReadFile(GeneratedDirectory() / "OpenGL" / (std::string(name) + ".glsl"));
		ASSERT_FALSE(glsl.empty()) << name;
		EXPECT_TRUE(glsl.starts_with("#version 410")) << name;
		EXPECT_EQ(glsl.find("binding"), std::string::npos) << name;
		EXPECT_EQ(glsl.find("origin_upper_left"), std::string::npos) << name;
	}
}
#endif

#if CE_RENDER_METAL
/**
 * @brief The MSL functions carry the names MetalShaderModule looks up, and the bindings became Metal indices
 */
TEST(ShaderReflectionTest, GeneratedMsl_FollowsEntryPointAndSlotConventions) {
	const auto vertex = ReadFile(GeneratedDirectory() / "Metal" / "Vertex.metal");
	EXPECT_NE(vertex.find("vertexMain("), std::string::npos);
	EXPECT_NE(vertex.find("[[buffer(" + std::to_string(CE::Types::CAMERA_CONSTANTS_SLOT) + ")]]"), std::string::npos);
	EXPECT_NE(vertex.find("[[buffer(" + std::to_string(CE::Types::OBJECT_CONSTANTS_SLOT) + ")]]"), std::string::npos);

	const auto composite = ReadFile(GeneratedDirectory() / "Metal" / "CompositeFragment.metal");
	EXPECT_NE(composite.find("compositeFragmentMain("), std::string::npos);
	EXPECT_NE(composite.find("[[texture(0)]]"), std::string::npos);
}
#endif
