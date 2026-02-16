#include <CelestialEngine.hpp>
#include <Events/ApplicationEvent.hpp>
#include <Events/MouseEvent.hpp>
#include <Tools/Log/Log.hpp>

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
	
public:
	void Run() override {
		Application::Run();
		const Events::MouseButtonPressedEvent event{1280};
		if (event.IsInCategory(Events::EventCategoryMouse)) {
			CE_TRACE(event);
		}
		if (event.IsInCategory(Events::EventCategoryMouseButton)) {
			CE_TRACE(event);
		}
		if (event.IsInCategory(Events::EventCategoryInput)) {
			CE_TRACE(event);
		}
	}
};

Core::Application* Core::CreateApplication() {
	return new SandBox();
}
