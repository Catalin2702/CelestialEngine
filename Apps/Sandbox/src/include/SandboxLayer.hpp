//
// Module: CelestialEngine/Apps/Sandbox
// File: SandboxLayer.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-03
// Updated by: Catalin Chirosca
// Updated: 2026-11-27
//

#pragma once

#ifndef APPS_SANDBOX_SANDBOXLAYER_HPP
#define APPS_SANDBOX_SANDBOXLAYER_HPP

#include <CelestialEngine.hpp>

#include <memory>
#include <functional>


namespace Sandbox {

class SandboxLayer final: public CE::Core::I_Layer {
public:
	explicit SandboxLayer(CE::Core::I_Renderer& renderer);

public:
	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate() override {}
	void OnRender() const override {}

	void OnRenderScene(CE::Core::I_Renderer& renderer) const override;

private:
	void _CreateCube(CE::RHI::I_GraphicDevice& graphicDevice);
	/// Loads the logo and builds everything the textured quad is drawn with; leaves it all null if the image is missing.
	void _CreateTexturedQuad(CE::RHI::I_GraphicDevice& graphicDevice);

private:
	std::reference_wrapper<CE::Core::I_Renderer> _renderer;

	std::shared_ptr<CE::RHI::I_PipelineState> _pipeline;
	std::shared_ptr<CE::RHI::I_VertexBuffer> _vertexBuffer;
	std::shared_ptr<CE::RHI::I_IndexBuffer> _indexBuffer;

	std::shared_ptr<CE::RHI::I_PipelineState> _texturedPipeline;
	std::shared_ptr<CE::RHI::I_VertexBuffer> _texturedVertexBuffer;
	std::shared_ptr<CE::RHI::I_IndexBuffer> _texturedIndexBuffer;
	std::shared_ptr<CE::RHI::I_Texture> _texture;
	std::shared_ptr<CE::RHI::I_Sampler> _sampler;
};

}

#endif //APPS_SANDBOX_SANDBOXLAYER_HPP
