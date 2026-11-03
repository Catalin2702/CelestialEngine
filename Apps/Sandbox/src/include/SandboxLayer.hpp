//
// Module: CelestialEngine/Apps/Sandbox
// File: SandboxLayer.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-03
// Updated by: Catalin Chirosca
// Updated: 2026-11-03
//

#pragma once

#ifndef APPS_SANDBOX_SANDBOXLAYER_HPP
#define APPS_SANDBOX_SANDBOXLAYER_HPP

#include <CelestialEngine.hpp>

#include <memory>


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
	CE::Core::I_Renderer& _renderer;

	std::shared_ptr<CE::Core::I_PipelineState> _pipeline;
	std::shared_ptr<CE::Core::I_VertexBuffer> _vertexBuffer;
	std::shared_ptr<CE::Core::I_IndexBuffer> _indexBuffer;
};

}

#endif //APPS_SANDBOX_SANDBOXLAYER_HPP
