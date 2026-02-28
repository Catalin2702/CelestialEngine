//
// Created by Catalin Chirosca on 2026-02-19.
//

#include "Layers/I_Layer.hpp"


namespace CE::Layers {

I_Layer::I_Layer([[maybe_unused]] const std::string& name) {
#ifdef CE_DEBUG
	_debugName = name;
#endif
}

}
