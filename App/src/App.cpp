#include <CelestialEngine.hpp>


namespace Core = CE::Core;
namespace Events = CE::Events;
namespace Log = CE::Tools::Log;
namespace Layers = CE::Layers;

class ExampleLayer : public Layers::Layer {
public:
	ExampleLayer(): Layer("ExampleLayer") {}

	void OnUpdate() override {
		CE_INFO("ExampleLayer::Update");
	}

	void OnEvent([[maybe_unused]] const Events::Event &event) override {
		CE_INFO("{0}", event);
	}

	void OnAttach() override {}
	void OnDetach() override {}
};

class SandBox final: public Core::Application {
public:
	SandBox() {
		PushLayer(new ExampleLayer());
	}
	~SandBox() override {
		CE_CORE_INFO("Sandbox destructed");
	}
};

Core::Application* Core::CreateApplication() {
	return new SandBox();
}
