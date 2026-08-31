//
// Module: CelestialEngine/Engine/Modules/Types/Render
// File: AttachmentAction.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-31
// Updated by: Catalin Chirosca
// Updated: 2026-08-31
//

#include "Types/Render/AttachmentAction.hpp"


namespace CE::Types {

const char* ToString(const LoadAction loadAction) {
	switch (loadAction) {
		case LoadAction::Load: return "Load";
		case LoadAction::Clear: return "Clear";
		case LoadAction::DontCare: return "DontCare";
		default: return "Unknown";
	}
}

const char* ToString(const StoreAction storeAction) {
	switch (storeAction) {
		case StoreAction::Store: return "Store";
		case StoreAction::DontCare: return "DontCare";
		default: return "Unknown";
	}
}

}
