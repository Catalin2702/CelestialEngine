//
// Created by Catalin Chirosca on 2026-02-15.
//

#ifndef CELESTIALENGINE_APPLICATION_HPP
#define CELESTIALENGINE_APPLICATION_HPP

#include <Core/Core.hpp>

namespace CE::Core {
	class CE_API Application {
	public:
		Application();
		virtual ~Application();

	public:
		void Run();
	};
}

#endif //CELESTIALENGINE_APPLICATION_HPP
