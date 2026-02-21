//
// Created by Catalin Chirosca on 2026-02-19.
//

#include "Layers/Layer.hpp"


namespace CE::Layers {

Layer::Layer([[maybe_unused]] const std::string& name) {
#ifdef CE_DEBUG
	_name = name;
#endif
}

}
