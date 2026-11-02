//
// Module: CelestialEngine/Engine/Modules/Utility/Callback
// File: Dispatcher.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#include "Utility/Delegate/Dispatcher.hpp"


namespace CE::Utility::Detail {

void ReportDelegateAlreadyBound() {
	CE_CORE_WARN("A delegate is already assigned");
}

void ThrowCallbackNotBound() {
	constexpr auto error = "CallbackDispatcher::Execute: Set the callback before trying to execute this method.";
	CE_CORE_ERROR(error);
	throw std::runtime_error(error);
}

}
