//
// Module: CelestialEngine/Apps/Sandbox
// File: SandboxLayer.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-03
// Updated by: Catalin Chirosca
// Updated: 2026-11-27
//

#include "SandboxLayer.hpp"

namespace Sandbox {

SandboxLayer::SandboxLayer(CE::Core::I_Renderer& renderer):
	I_Layer("Sandbox"),
	_renderer(renderer) {
}

void SandboxLayer::OnAttach() {
	using namespace CE::IO;
	// The application's own files are read through resources://, the Content folder ce_configure_application shipped
	// (CONTENT in this directory's CMakeLists.txt).
	if (const auto welcome = FileSystem::ReadText(VirtualPath::Compose(Mount::Resources, "Welcome.txt"))) {
		CE_INFO("SandboxLayer::OnAttach: {}", *welcome);
	}
	else {
		CE_WARN("SandboxLayer::OnAttach: Cannot read resources://Welcome.txt ({})", welcome.error());
	}

	auto& graphicDevice = _renderer.get().GetGraphicDevice();

	_CreateTexturedQuad(graphicDevice);
	_CreateCube(graphicDevice);
}

void SandboxLayer::OnDetach() {
	// Released here, at a moment the application controls, rather than whenever the last owner of the layer lets go.
	_pipeline.reset();
	_vertexBuffer.reset();
	_indexBuffer.reset();

	_texturedPipeline.reset();
	_texturedVertexBuffer.reset();
	_texturedIndexBuffer.reset();
	_texture.reset();
	_sampler.reset();
}

void SandboxLayer::OnRenderScene(CE::Core::I_Renderer& renderer) const {
	if (_pipeline and _vertexBuffer and _indexBuffer) {
		CE::Core::DrawCommand command{};
		command.pipelineState = _pipeline.get();
		command.vertexBuffer = _vertexBuffer.get();
		command.indexBuffer = _indexBuffer.get();

		renderer.Submit(command);
	}

	// The logo is optional: a missing file leaves it all null, and the rest of the scene still draws.
	if (_texturedPipeline and _texture and _sampler) {
		const CE::Core::DrawCommand texturedCommand{
			.pipelineState = _texturedPipeline.get(),
			.vertexBuffer = _texturedVertexBuffer.get(),
			.indexBuffer = _texturedIndexBuffer.get(),
			.texture = _texture.get(),
			.sampler = _sampler.get()
		};

		renderer.Submit(texturedCommand);
	}
}
void SandboxLayer::_CreateCube(CE::RHI::I_GraphicDevice& graphicDevice) {
	// x, y, z, r, g, b, a - centred on the origin, which the camera looks at: x and y span ±0.625 and ±0.825. One vertex
	// per row: clang-format would otherwise put every number on a line of its own.
	// clang-format off
	constexpr std::array vertices{
		// Front face (green)
		-0.625f, -0.825f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 0
		+0.475f, -0.825f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 1
		+0.475f, +0.675f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 2
		-0.625f, +0.675f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 3

		// Lateral face (red)
		+0.475f, -0.825f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 4
		+0.625f, -0.675f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 5
		+0.625f, +0.825f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 6
		+0.475f, +0.675f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 7

		// Upper face (blue)
		+0.475f, +0.675f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 8
		+0.625f, +0.825f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 9
		-0.475f, +0.825f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 10
		-0.625f, +0.675f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 11
	};
	// clang-format on

	constexpr std::array indices{
		0_u32, 1_u32, 2_u32, 2_u32, 3_u32, 0_u32, // Front face
		4_u32, 5_u32, 6_u32, 6_u32, 7_u32, 4_u32, // Lateral face
		8_u32, 9_u32, 10_u32, 10_u32, 11_u32, 8_u32 // Upper face
	};

	// Named, because it is needed twice: the buffer uses it to compute its stride, and the pipeline needs it to build
	// its vertex descriptor. Backends that bake that descriptor into the compiled pipeline draw nothing without it,
	// while those that read the layout at bind time never miss it - so leaving it off is a bug that hides on some.
	const CE::RHI::BufferLayout vertexLayout{
		{CE::Types::ShaderDataType::Float3, "inputPosition"},
		{CE::Types::ShaderDataType::Float4, "inputColor"}
	};

	_vertexBuffer = graphicDevice.CreateVertexBuffer(vertices, vertexLayout);
	_indexBuffer = graphicDevice.CreateIndexBuffer(indices);

	// Named, not described: which artifact these two names stand for, and where it lives, is the device's to work out.
	CE::RHI::PipelineDescriptor pipelineDescriptor{};
	pipelineDescriptor.vertexLayout = vertexLayout;
	pipelineDescriptor.vertexShader = graphicDevice.CreateShaderModule({.stage = CE::Types::ShaderType::Vertex, .name = "Vertex"});
	pipelineDescriptor.fragmentShader = graphicDevice.CreateShaderModule({.stage = CE::Types::ShaderType::Fragment, .name = "Fragment"});

	// Asked rather than assumed: most backends reject a pipeline whose format disagrees with what it draws into.
	pipelineDescriptor.formats.colors[0] = _renderer.get().GetSceneColorFormat();
	pipelineDescriptor.formats.colorCount = 1;

	_pipeline = graphicDevice.CreatePipelineState(pipelineDescriptor);
}
void SandboxLayer::_CreateTexturedQuad(CE::RHI::I_GraphicDevice& graphicDevice) {
	using namespace CE;

	// Shipped from Apps/Sandbox/Content/Textures by CONTENT in this directory's CMakeLists.txt.
	const auto image = Core::Image::Load(IO::VirtualPath::Compose(IO::Mount::Resources, "Textures/Logo.png"));
	if (not image) [[unlikely]]
		return; // Image::Load has already said why.

	// Sampled only: nothing renders into it, and saying so is what lets a backend skip render-target layouts.
	_texture = graphicDevice.CreateTexture({
		.width = image->width,
		.height = image->height,
		.format = image->format,
		.usage = Types::TextureUsage::ShaderRead,
		.debugName = "Logo",
		.initialData = image->pixels,
	});

	_sampler = graphicDevice.CreateSampler({
		.minFilter = Types::FilterMode::Linear,
		.magFilter = Types::FilterMode::Linear,
		.addressU = Types::AddressMode::ClampToEdge,
		.addressV = Types::AddressMode::ClampToEdge,
		.debugName = "LogoSampler"
	});

	// x, y, z, u, v - centred on the origin like the test geometry, and in front of it (z = 1). v = 0 is the image's
	// first row, its top, on every backend: the top corners get v = 0, and nothing has to be flipped anywhere.
	// clang-format off
	constexpr std::array vertices{
		-0.3f, -0.3f, 1.0f, 0.0f, 1.0f, // 0 bottom-left
		+0.3f, -0.3f, 1.0f, 1.0f, 1.0f, // 1 bottom-right
		+0.3f, +0.3f, 1.0f, 1.0f, 0.0f, // 2 top-right
		-0.3f, +0.3f, 1.0f, 0.0f, 0.0f, // 3 top-left
	};
	// clang-format on

	constexpr std::array indices{0_u32, 1_u32, 2_u32, 2_u32, 3_u32, 0_u32};

	const RHI::BufferLayout vertexLayout{
		{Types::ShaderDataType::Float3, "inputPosition"},
		{Types::ShaderDataType::Float2, "inputTexCoord"},
	};

	_texturedVertexBuffer = graphicDevice.CreateVertexBuffer(vertices, vertexLayout);
	_texturedIndexBuffer = graphicDevice.CreateIndexBuffer(indices);

	RHI::PipelineDescriptor pipelineDescriptor{};
	pipelineDescriptor.vertexLayout = vertexLayout;
	pipelineDescriptor.vertexShader = graphicDevice.CreateShaderModule({
		.stage = Types::ShaderType::Vertex,
		.name = "TexturedVertex"
	});
	pipelineDescriptor.fragmentShader = graphicDevice.CreateShaderModule({
		.stage = Types::ShaderType::Fragment,
		.name = "TexturedFragment"
	});

	// The default blend state is straight alpha, which is what lets the logo's transparent corners show the clear
	// colour through.
	pipelineDescriptor.formats.colors[0] = _renderer.get().GetSceneColorFormat();
	pipelineDescriptor.formats.colorCount = 1;

	_texturedPipeline = graphicDevice.CreatePipelineState(pipelineDescriptor);
}

}
