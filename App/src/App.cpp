#include <CelestialEngine.hpp>


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
	SandBox(const TypeWindow::WindowProps& windowProps):Application(windowProps) {
		PushLayer(new ExampleLayer());
	}
	SandBox(const std::string &title, const unsigned int width, const unsigned int height, const bool VSync): Application(title, width, height, VSync) {
		PushLayer(new ExampleLayer());
	}
	~SandBox() override {
		CE_CORE_INFO("Sandbox destructed");
	}
};

Core::Application* Core::CreateApplication() {
	return new SandBox();
}

Core::Application* CE::Core::CreateApplication(const CeTypeWindow::WindowProps& windowProps) {
	return new SandBox(windowProps);
}

Core::Application* Core::CreateApplication(const std::string& title, const unsigned int width, const unsigned int height, const bool VSync) {
	return new SandBox(title, width, height, VSync);
}
