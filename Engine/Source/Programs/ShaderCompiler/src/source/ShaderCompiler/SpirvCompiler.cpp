//
// Module: CelestialEngine/Engine/Source/Programs/ShaderCompiler
// File: SpirvCompiler.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-26
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

#include "ShaderCompiler/SpirvCompiler.hpp"

#include <glslang/Public/ResourceLimits.h>
#include <glslang/Public/ShaderLang.h>
#include <glslang/SPIRV/GlslangToSpv.h>

namespace CE::ShaderCompiler {

namespace {

/// The #version the sources are written against when they forget to say it. Every engine shader states it anyway.
constexpr u32 DefaultGlslVersion = 450;

EShLanguage ToGlslang(const ShaderStage stage) {
	switch (stage) {
		case ShaderStage::Vertex: return EShLangVertex;
		case ShaderStage::Fragment: return EShLangFragment;
		default: return EShLangVertex;
	}
}

/**
 * @brief glslang's process-wide tables, set up for exactly as long as one compilation needs them
 * @details InitializeProcess/FinalizeProcess are reference counted, so nesting or repeating them is safe.
 */
struct GlslangProcess {
	GlslangProcess() { glslang::InitializeProcess(); }
	~GlslangProcess() { glslang::FinalizeProcess(); }

	GlslangProcess(const GlslangProcess&) = delete;
	GlslangProcess& operator = (const GlslangProcess&) = delete;
};

}

std::expected<SpirvBinary, std::string> CompileToSpirv(const std::string_view source, const std::string& sourceName, const ShaderStage stage) {
	const GlslangProcess process{};
	const auto language = ToGlslang(stage);

	glslang::TShader shader{language};
	const char* const sText = source.data();
	const auto sLength = static_cast<i32>(source.size());
	const char* const sName = sourceName.c_str();
	shader.setStringsWithLengthsAndNames(&sText, &sLength, &sName, 1);

	// Vulkan GLSL in, SPIR-V 1.0 out: the most widely accepted version, and all SPIRV-Cross needs. "Vulkan" here is the
	// dialect of the source - explicit set/binding on every resource - not a promise that Vulkan will run it.
	shader.setEnvInput(glslang::EShSourceGlsl, language, glslang::EShClientVulkan, 100);
	shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_0);
	shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_0);
	shader.setEntryPoint("main");

	constexpr auto messages = static_cast<EShMessages>(EShMsgSpvRules | EShMsgVulkanRules);
	if (not shader.parse(GetDefaultResources(), DefaultGlslVersion, false, messages)) [[unlikely]]
		return std::unexpected(std::string(shader.getInfoLog()));

	// Linking a single stage still matters: it is what resolves the entry point and checks the interface as a whole.
	glslang::TProgram program;
	program.addShader(&shader);
	if (not program.link(messages)) [[unlikely]]
		return std::unexpected(std::string(program.getInfoLog()));

	SpirvBinary spirv;
	// Names (OpName) are kept: SPIRV-Cross writes them back into the MSL and GLSL, which is what keeps the
	// generated code readable - and what OpenGL binds uniform blocks and samplers by.
	glslang::SpvOptions options{
		.generateDebugInfo = false,
		.stripDebugInfo = false,
		.disableOptimizer = true,
	};

	glslang::GlslangToSpv(*program.getIntermediate(language), spirv, &options);

	return spirv;
}

}
