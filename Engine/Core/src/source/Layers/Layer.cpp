//
// Created by Catalin Chirosca on 2026-02-19.
//

#include "Layers/Layer.hpp"


namespace CE::Layers {

Layer::Layer([[maybe_unused]] std::string name) {
#ifdef CE_DEBUG
	_name = std::move(name);
#endif
}

}
