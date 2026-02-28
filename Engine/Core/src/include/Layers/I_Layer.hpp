//
// Created by Catalin Chirosca on 2026-02-19.
//

#pragma once

#ifndef CE_LAYERS_I_LAYER_HPP
#define CE_LAYERS_I_LAYER_HPP

#include <Define/DynamicLinker.hpp>
#include <Events/I_Event.hpp>


namespace CE::Layers {

class CE_API I_Layer {
public:
	I_Layer([[maybe_unused]] const std::string& name = "Layer");
	virtual ~I_Layer() = default;

public:
	virtual void OnAttach() = 0;
	virtual void OnDetach() = 0;
	virtual void OnUpdate() = 0;
	virtual bool OnEvent(Events::I_Event& event) = 0;

#ifdef CE_DEBUG
public:
	[[nodiscard]] inline const std::string& GetDebugName() const { return _debugName; }

protected:
	std::string _debugName;
#endif

};

}

#endif //CE_LAYERS_I_LAYER_HPP
