//
// Module: CelestialEngine/Apps/Sandbox
// File: App.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-16
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

// The sandbox's entry point. EntryPoint.hpp defines main(), so it is included here and nowhere else: every other
// file of the application includes CelestialEngine.hpp alone.

#include "SandboxLayer.hpp"

#include <CelestialEngine.hpp>
// ReSharper disable once CppUnusedIncludeDirective
#include <EntryPoint.hpp>

#include <memory>

namespace Sandbox {

namespace {

using namespace CE;

/**
 * @class SandboxClient
 * @brief The engine's test bed: a free-flying camera over the test geometry
 */
class SandboxClient final: public  Core::I_ApplicationClient {
public:
	[[nodiscard]] Core::ApplicationSpecification GetSpecification() const override {
		return {};
	}
public:
	void OnInit(Core::Application& application) override {
		_PushCamera(application);

		application.PushLayer(std::make_shared<SandboxLayer>(application.GetRenderer()));
	}

	void OnUpdate(Core::Application&, f32) override {}

	void OnShutdown(Core::Application&) override {}

private:
	static void _PushCamera(Core::Application& application) {
		const auto& windowProps = Utility::Config::GetWindowProps();

		constexpr Core::CameraDescriptor cameraDescriptor{
			.position = {0.0_f32, 0.0_f32, 5.0_f32},
		};

		const Core::CameraProjectionDescriptor projectionDescriptor{
			.type =  Types::CameraProjection::Orthographic,
			// Backends disagree on the depth range clip space maps onto: asked of the device, which knows.
			.convention = application.GetRenderer().GetGraphicDevice().GetClipConvention(),
			.viewportWidth = windowProps.width,
			.viewportHeight = windowProps.height,
			// The test geometry spans about ±0.9, so a box one unit tall above and below the centre frames it.
			.orthographicSize = 1.0_f32
		};

		constexpr Core::CameraControllerDescriptor controllerDescriptor{
			.descriptor =  Core::CameraFreeControllerDescriptor{
				.move = {.speed = 1._f32},
				.look = {}
			}
		};

		const auto sceneLayer = std::make_shared<Core::SceneLayer>(cameraDescriptor, projectionDescriptor, controllerDescriptor);

		application.PushLayer(sceneLayer);
		sceneLayer->SubscribeToEventHub(application.GetEventHubDispatcher());
	}
};

}

}

namespace CE {

std::unique_ptr<Core::I_ApplicationClient> CreateApplicationClient() {
	return std::make_unique<Sandbox::SandboxClient>();
}

}
