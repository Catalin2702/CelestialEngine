#include <CelestialEngine.hpp>
#include <memory>


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
		_Init();
	}
	SandBox(const TypeWindow::WindowProps& windowProps):Application(windowProps) {
		_Init();
	}
	SandBox(const std::string &title, const unsigned int width, const unsigned int height, const bool VSync): Application(title, width, height, VSync) {
		_Init();
	}
	~SandBox() override = default;

private:
	void _Init() {
		auto layer = std::make_unique<ExampleLayer>();
		PushLayer(layer.release());
	}
};

Core::Application* Core::CreateApplication() {
	auto app = std::make_unique<SandBox>();
	return app.release();
}

Core::Application* Core::CreateApplication(const CeTypeWindow::WindowProps& windowProps) {
	auto app = std::make_unique<SandBox>(windowProps);
	return app.release();
}

Core::Application* Core::CreateApplication(const std::string& title, const unsigned int width, const unsigned int height, const bool VSync) {
	auto app = std::make_unique<SandBox>(title, width, height, VSync);
	return app.release();
}
