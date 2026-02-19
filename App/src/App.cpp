#include <CelestialEngine.hpp>


namespace Core = CE::Core;
namespace Events = CE::Events;
namespace Log = CE::Tools::Log;

class SandBox final: public Core::Application {
public:
	SandBox() {
		CE_CORE_INFO("Sandbox created");
	}
	~SandBox() override {
		CE_CORE_INFO("Sandbox destructed");
	}
};

Core::Application* Core::CreateApplication() {
	return new SandBox();
}
