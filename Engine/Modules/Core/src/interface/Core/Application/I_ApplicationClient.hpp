//
// Module: CelestialEngine/Engine/Modules/Core/Application
// File: I_ApplicationClient.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-03
// Updated by: Catalin Chirosca
// Updated: 2026-11-03
//

#pragma once

#ifndef CE_CORE_APPLICATION_I_APPLICATIONCLIENT_HPP
#define CE_CORE_APPLICATION_I_APPLICATIONCLIENT_HPP

#include "Core/Application/ApplicationSpecification.hpp"
#include "Define/DynamicLinker.hpp"
#include "Types/Var/Vars.hpp"

#include <memory>

namespace CE {

namespace Core {

class Application;

/**
 * @class I_ApplicationClient
 * @brief The executable's side of the application: what a sandbox, an editor or a game adds to the engine
 * @details The engine owns the loop and calls in at fixed points - the client never drives. Every executable built on
 *			the engine implements this once and returns it from CE::CreateApplicationClient; the one Application owns it
 *			from then on. Products differ by their client; inside one product, the client composes layers.
 */
class CE_CORE_API I_ApplicationClient {
public:
	virtual ~I_ApplicationClient() = default;

public:
	/**
	 * @brief Gets the choices the engine needs before anything exists
	 * @details Asked once, by the Application constructor - so it must not reach for the window or the renderer.
	 */
	[[nodiscard]] virtual ApplicationSpecification GetSpecification() const = 0;

	/**
	 * @brief Called once the window and the renderer exist: push the client's layers here
	 * @details Fired from the platform's ready signal, before the first frame and before the ImGui overlay is built.
	 */
	virtual void OnInit(Application& application) = 0;

	/**
	 * @brief Called every frame, before the layers update
	 * @details For logic that belongs to the product rather than to one layer. Runs outside the iteration over the
	 *			layer stack, so it is the safe place to push, pop or replace layers.
	 */
	virtual void OnUpdate(Application& application, f32 deltaTime) = 0;

	/**
	 * @brief Called once when the application ends, while the layers, the hub and the renderer are still alive
	 * @details Only after an OnInit: a platform that never became ready never initialised the client either.
	 */
	virtual void OnShutdown(Application& application) = 0;
};

}

[[nodiscard]] std::unique_ptr<Core::I_ApplicationClient> CreateApplicationClient();

}

#endif //CE_CORE_APPLICATION_I_APPLICATIONCLIENT_HPP
