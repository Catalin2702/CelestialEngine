#include <CelestialEngine.hpp>


class ExampleLayer : public Layers::Layer {
public:
	ExampleLayer(): Layer("ExampleLayer") {}

	void OnUpdate() override {}

	void OnEvent([[maybe_unused]] Events::Event &event) override {}

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
	~SandBox() override = default;
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
