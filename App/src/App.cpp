#include <CelestialEngine.hpp>
#include <print>

namespace CeCore = CE::Core;

class SandBox: public CeCore::Application {
public:
	SandBox() {
		std::println("Sandbox created");
	}
	~SandBox() override {
		std::println("Sandbox destructed");
	}
};

CeCore::Application* CeCore::CreateApplication() {
	return new SandBox();
}
