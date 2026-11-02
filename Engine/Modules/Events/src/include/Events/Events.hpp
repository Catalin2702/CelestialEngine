//
// Module: CelestialEngine/Engine/Modules/Events
// File: Events.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

/**
 * @file Events.hpp
 * @brief Umbrella header for the Events module
 * @details One include for the event interface and every concrete event: application, generic (error), keyboard, mouse,
 *			render and window.
 */

#pragma once

#ifndef CE_EVENTS_EVENTS_HPP
#define CE_EVENTS_EVENTS_HPP

#include "Events/I_Event.hpp"

#include "Events/ApplicationEvent.hpp"
#include "Events/GenericEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Events/RenderEvent.hpp"
#include "Events/WindowEvent.hpp"

#endif //CE_EVENTS_EVENTS_HPP
