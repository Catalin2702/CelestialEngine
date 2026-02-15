#include <CelestialEngine.hpp>
#include <print>

class SandBox: public CE::Core::Application {
public:
	SandBox() {
		std::println("Sandbox created");
	}
	~SandBox() override {
		std::println("Sandbox destructed");
	}
};

CE::Core::Application* CE::Core::CreateApplication() {
	return new SandBox();
}
