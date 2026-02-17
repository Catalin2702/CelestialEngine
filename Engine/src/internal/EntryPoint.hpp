//
// Created by Catalin Chirosca on 2026-02-15.
//

#pragma once

#ifndef CE_ENTRYPOINT_HPP
#define CE_ENTRYPOINT_HPP

#ifdef CE_API

namespace CeCore = CE::Core;
namespace CeLog = CE::Tools::Log;
namespace CeTime = CE::Tools::Time;


int main() {
	{
		[[maybe_unused]] CeTime::Timer timer;
		CeLog::Log::Init();
		const auto app = CeCore::CreateApplication();
		app->Run();
		CeLog::Log::Terminate();
		delete app;
	}
}

#endif

#endif //CE_ENTRYPOINT_HPP
