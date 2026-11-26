//
// Module: CelestialEngine/Engine/Source/Programs/ShaderCompiler
// File: main.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-26
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

#include "ShaderCompiler/CrossCompiler.hpp"
#include "ShaderCompiler/Reflection.hpp"
#include "ShaderCompiler/ShaderStage.hpp"
#include "ShaderCompiler/SpirvCompiler.hpp"
#include "Types/Var/Vars.hpp"

#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <optional>
#include <span>
#include <string>
#include <string_view>

namespace fs = std::filesystem;
using namespace CE::ShaderCompiler;


namespace {

/// What one run is asked to do: one source in, any combination of outputs.
struct Arguments {
	fs::path input;
	std::string name;
	std::optional<fs::path> outSpirv;
	std::optional<fs::path> outMsl;
	std::optional<fs::path> outGlsl;
	std::optional<fs::path> outReflection;
};

constexpr auto Usage =
	"usage: CE_ShaderCompiler --input <Name.vert|Name.frag> [--name <Name>]\n"
	"						  [--spirv <out.spv>] [--msl <out.metal>] [--glsl <out.glsl>] [--reflection <out.reflect]\n";

/// Reports a failure the way compilers do, so IDEs and build logs show it as an error line.
int Fail(const std::string_view message) {
	std::cerr << "CE_ShaderCompiler - Fail: error: " << message << "\n";
	return EXIT_FAILURE;
}

std::optional<Arguments> ParseArguments(const std::span<char*> args) {
	Arguments arguments;

	// Every option takes a value, so they are read in pairs; an option in last position has none and is an error.
	for (std::size_t i = 1; i < args.size(); i += 2) {
		const std::string_view option = args[i];
		if (i + 1 >= args.size())
			return std::nullopt;
		const std::string_view value = args[i + 1];

		if (option == "--input")
			arguments.input = value;
		else if (option == "--name")
			arguments.name = value;
		else if (option == "--spirv")
			arguments.outSpirv = value;
		else if (option == "--msl")
			arguments.outMsl = value;
		else if (option == "--glsl")
			arguments.outGlsl = value;
		else if (option == "--reflection")
			arguments.outReflection = value;
		else
			return std::nullopt;
	}

	if (arguments.input.empty())
		return std::nullopt;

	// The engine's name for the shader is the file's: "CompositeVertex.vert" is "CompositeVertex".
	if (arguments.name.empty())
		arguments.name = arguments.input.stem().string();

	return arguments;
}

/// "CompositeVertex" -> "compositeVertexMain": MetalShaderModule::ResolveEntryPoint's convention, from the other side.
std::string MetalEntryPoint(std::string name) {
	name.front() = static_cast<char>(std::tolower(static_cast<unsigned char>(name.front())));
	return name + "Main";
}

std::optional<std::string> ReadText(const fs::path& path) {
	std::ifstream file(path, std::ios::binary);
	if (not file)
		return std::nullopt;

	return std::string(std::istreambuf_iterator(file), std::istreambuf_iterator<char>());
}

/// Writes the whole buffer, creating the folders on the way. Always rewritten, even when unchanged: the build compares
/// timestamps, and an output older than its source would make it run this tool again on every build.
bool WriteBytes(const fs::path& path, const void* data, const std::size_t size) {
	std::error_code errorCode;
	fs::create_directories(path.parent_path(), errorCode);

	std::ofstream file(path, std::ios::binary | std::ios::trunc);
	file.write(static_cast<const char*>(data), static_cast<std::streamsize>(size));
	return static_cast<bool>(file);
}

bool WriteText(const fs::path& path, const std::string& text) {
	return WriteBytes(path, text.data(), text.size());
}

}

int main(const int argc, char* argv[]) {
	const auto arguments = ParseArguments(std::span(argv, static_cast<std::size_t>(argc)));
	if (not arguments) {
		std::cerr << Usage;
		return EXIT_FAILURE;
	}

	const auto stage = StageFromExtension(arguments->input);
	if (not stage)
		return Fail(arguments->input.string() + ": the extension does not name a stage (.vert, .frag)!");

	const auto source = ReadText(arguments->input);
	if (not source)
		return Fail(arguments->input.string() + ": cannot be read!");

	// 1. Front end: GLSL -> SPIR-V. Every error in the shader's code is reported here.
	// glslang's log already reads "ERROR: <file>:<line>: ...", the shape IDEs turn into links: printed as it is.
	const auto spirv = CompileToSpirv(source.value(), arguments->input.string(), stage.value());
	if (not spirv) {
		return Fail(spirv.error());
	}

	if (arguments->outSpirv and not WriteBytes(arguments->outSpirv.value(), spirv->data(), spirv->size() * sizeof(u32)))
		return Fail(arguments->outSpirv->string() + ": cannot be written!");

	// 2. Back ends: SPIR-V -> the language of each backend that was asked for.
	if (arguments->outMsl) {
		const auto msl = ToMsl(spirv.value(), stage.value(), MetalEntryPoint(arguments->name));
		if (not msl)
			return Fail(arguments->name + " (MSL): " + msl.error());
		if (not WriteText(arguments->outMsl.value(), msl.value()))
			return Fail(arguments->outMsl->string() + ": cannot be written!");
	}

	if (arguments->outGlsl) {
		const auto glsl = ToGlsl(spirv.value());
		if (not glsl)
			return Fail(arguments->name + " (GLSL): " + glsl.error());
		if (not WriteText(arguments->outGlsl.value(), glsl.value()))
			return Fail(arguments->outGlsl->string() + ": cannot be written!");
	}

	// 3. Reflection: what the shader declares, for the engine's tests to check against the C++ side.
	if (arguments->outReflection) {
		const auto reflection = Reflect(spirv.value(), stage.value());
		if (not reflection)
			return Fail(arguments->name + " (reflection): " + reflection.error());
		if (not WriteText(arguments->outReflection.value(), reflection.value()))
			return Fail(arguments->outReflection->string() + ": cannot be written!");
	}

	return EXIT_SUCCESS;
}
