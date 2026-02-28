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

/**
 * @brief Constructor implementation for I_Layer
 * @param name Debug name for the layer (only stored in debug builds)
 * @details In debug builds, stores the name for debugging purposes.
 *          In release builds, the parameter is unused and no storage occurs.
 */
I_Layer::I_Layer([[maybe_unused]] const std::string& name) {
#ifdef CE_DEBUG
	_debugName = name;
#endif
}

}
