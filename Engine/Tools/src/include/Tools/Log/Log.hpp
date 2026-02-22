//
// Created by Catalin Chirosca on 2026-02-16.
//

#pragma once

#ifndef CE_LOG_LOG_HPP
#define CE_LOG_LOG_HPP

#include "Define/DynamicLinker.hpp"
#include "Define/Log.hpp"

#include <memory>
#include <spdlog/spdlog.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <spdlog/fmt/ostr.h>

namespace CE::Tools::Log {

class CE_API Log {
public:
	static void Init();
	static void Terminate();

	static std::shared_ptr<spdlog::logger>& GetCoreLogger() {return _s_coreLogger; }
	static std::shared_ptr<spdlog::logger>& GetClientLogger() {return _s_clientLogger; }

private:
	static std::shared_ptr<spdlog::logger> _s_coreLogger;
	static std::shared_ptr<spdlog::logger> _s_clientLogger;
};

}


#endif //CE_LOG_LOG_HPP
