#include <CelestialEngine.hpp>

#include <print>

namespace Core = CE::Core;
namespace Events = CE::Events;
namespace Log = CE::Tools::Log;

class SandBox final: public Core::Application {
public:
	SandBox() {
		std::println("Sandbox created");
	}
	~SandBox() override {
		std::println("Sandbox destructed");
	}
};

Core::Application* Core::CreateApplication() {
	return new SandBox();
}
