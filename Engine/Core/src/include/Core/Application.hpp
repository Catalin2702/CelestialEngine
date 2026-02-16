//
// Created by Catalin Chirosca on 2026-02-15.
//

#pragma once

#ifndef CELESTIALENGINE_APPLICATION_HPP
#define CELESTIALENGINE_APPLICATION_HPP

#include "Core/Core.hpp"
#include "Events/Event.hpp"

namespace CE::Core {

class CE_API Application {
public:
	Application();

	virtual ~Application();

public:
	virtual void Run();
};

Application *CreateApplication();

}

#endif //CELESTIALENGINE_APPLICATION_HPP
