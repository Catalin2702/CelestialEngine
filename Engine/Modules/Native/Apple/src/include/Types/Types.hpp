//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Types
// File: Types.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-20
// Updated by: Catalin Chirosca
// Updated: 2026-05-20
//

#pragma once

#ifndef CE_NATIVE_APPLE_TYPES_TYPES_HPP
#define CE_NATIVE_APPLE_TYPES_TYPES_HPP

#include <functional>

namespace NS {
class Event;
class Notification;
}

using NativeEventCallback = std::function<void(NS::Event*)>;
using NativeNotificationCallback = std::function<void(NS::Notification*)>;
using VoidEventCallback = std::function<void()>;

#endif //CE_NATIVE_APPLE_TYPES_TYPES_HPP
