//
// Created by Catalin Chirosca on 2026-02-19.
//

#pragma once

#ifndef CE_LAYERS_LAYER_HPP
#define CE_LAYERS_LAYER_HPP

#include <Define/DynamicLinker.hpp>
#include <Events/Event.hpp>


namespace CE::Layers {

class CE_API Layer {
public:
	Layer([[maybe_unused]] const std::string& name = "Layer");
	virtual ~Layer() = default;

public:
	virtual void OnAttach() = 0;
	virtual void OnDetach() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnEvent(Events::Event& event) = 0;

#ifdef CE_DEBUG
public:
	[[nodiscard]] inline const std::string& GetName() const { return _name; }

protected:
	std::string _name;
#endif

};

}

#endif //CE_LAYERS_LAYER_HPP
