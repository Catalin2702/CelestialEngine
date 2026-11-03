//
// Module: CelestialEngine/Apps/Sandbox
// File: SandboxLayer.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-03
// Updated by: Catalin Chirosca
// Updated: 2026-11-03
//

#include "SandboxLayer.hpp"

namespace Sandbox {

SandboxLayer::SandboxLayer(CE::Core::I_Renderer& renderer): I_Layer("Sandbox"),
															_renderer(renderer) {
}

void SandboxLayer::OnAttach() {
	auto& graphicDevice = _renderer.GetGraphicDevice();

	constexpr std::array vertices{
		// Front face
		-0.55f, -0.75f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 0
		0.55f, -0.75f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 1
		0.55f, 0.75f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 2
		-0.55f, 0.75f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 3

		// Lateral face
		0.55f, -0.75f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 4
		0.70f, -0.60f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 5
		0.70f, 0.90f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 6
		0.55f, 0.75f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 7

		// Upper face
		0.55f, 0.75f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 8
		0.70f, 0.90f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 9
		-0.40f, 0.90f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 10
		-0.55f, 0.75f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 11
	};

	constexpr std::array indices{
		0_u32, 1_u32, 2_u32, 2_u32, 3_u32, 0_u32, // Front face
		4_u32, 5_u32, 6_u32, 6_u32, 7_u32, 4_u32, // Lateral face
		8_u32, 9_u32, 10_u32, 10_u32, 11_u32, 8_u32 // Upper face
	};

	// Named, because it is needed twice: the buffer uses it to compute its stride, and the pipeline needs it to build
	// its vertex descriptor. Backends that bake that descriptor into the compiled pipeline draw nothing without it,
	// while those that read the layout at bind time never miss it - so leaving it off is a bug that hides on some.
	const CE::Core::BufferLayout vertexLayout{
		{CE::Types::ShaderDataType::Float3, "inputPosition"},
		{CE::Types::ShaderDataType::Float4, "inputColor"}
	};

	_vertexBuffer = graphicDevice.CreateVertexBuffer(vertices, vertexLayout);
	_indexBuffer = graphicDevice.CreateIndexBuffer(indices);

	// Named, not described: which artifact these two names stand for, and where it lives, is the device's to work out.
	CE::Core::PipelineDescriptor pipelineDescriptor{};
	pipelineDescriptor.vertexLayout = vertexLayout;
	pipelineDescriptor.vertexShader = graphicDevice.CreateShaderModule({
		.stage = CE::Types::ShaderType::Vertex,
		.name = "Vertex"
	});
	pipelineDescriptor.fragmentShader = graphicDevice.CreateShaderModule({
		.stage = CE::Types::ShaderType::Fragment,
		.name = "Fragment"
	});

	// Asked rather than assumed: most backends reject a pipeline whose format disagrees with what it draws into.
	pipelineDescriptor.formats.colors[0] = _renderer.GetSceneColorFormat();
	pipelineDescriptor.formats.colorCount = 1;

	_pipeline = graphicDevice.CreatePipelineState(pipelineDescriptor);
}

void SandboxLayer::OnDetach() {
	// Released here, at a moment the application controls, rather than whenever the last owner of the layer lets go.
	_pipeline.reset();
	_vertexBuffer.reset();
	_indexBuffer.reset();
}

void SandboxLayer::OnRenderScene(CE::Core::I_Renderer& renderer) const {
	if (not (_pipeline and _vertexBuffer and _indexBuffer)) [[unlikely]]
		return;

	CE::Core::DrawCommand command{};
	command.pipelineState = _pipeline.get();
	command.vertexBuffer = _vertexBuffer.get();
	command.indexBuffer = _indexBuffer.get();

	renderer.Submit(command);
}

}
