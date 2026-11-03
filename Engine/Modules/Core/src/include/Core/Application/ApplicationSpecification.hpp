//
// Module: CelestialEngine/Engine/Modules/Core/Application
// File: ApplicationSpecification.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-03
// Updated by: Catalin Chirosca
// Updated: 2026-11-03
//

#pragma once

#ifndef CE_CORE_APPLICATION_APPLICATIONSPECIFICATION_HPP
#define CE_CORE_APPLICATION_APPLICATIONSPECIFICATION_HPP

#include "Define/DynamicLinker.hpp"


namespace CE::Core {

/**
 * @struct ApplicationSpecification
 * @brief What an application asks of the engine before anything is built
 * @details Read once, by the Application constructor, from I_ApplicationClient::GetSpecification. Only the choices the
 *			engine has to know before the window and the renderer exist belong here; everything that needs them is done
 *			in I_ApplicationClient::OnInit instead.
 */
struct CE_CORE_API ApplicationSpecification {
	bool imguiOverlay = true; ///< Build the ImGui overlay for the configured graphics API once the renderer is up
};

}

#endif //CE_CORE_APPLICATION_APPLICATIONSPECIFICATION_HPP
