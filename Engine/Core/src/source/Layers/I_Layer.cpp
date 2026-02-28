//
// Module: Layers
// File: I_Layer.cpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-19
// Updated: 2026-02-28
//

#include "Layers/I_Layer.hpp"


namespace CE::Layers {

I_Layer::I_Layer([[maybe_unused]] const std::string& name) {
#ifdef CE_DEBUG
	_debugName = name;
#endif
}

}
