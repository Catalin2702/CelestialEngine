//
// Created by Catalin Chirosca on 2026-02-15.
//

#pragma once

#ifndef WORKSPACE_ENTRYPOINT_HPP
#define WORKSPACE_ENTRYPOINT_HPP

#ifdef CE_API

namespace CeCore = CE::Core;
namespace CeLog = CE::Tools::Log;
namespace CeTime = CE::Tools::Time;


int main() {
	{
		CeTime::Timer timer;
		CeLog::Log::Init();
		const auto app = CeCore::CreateApplication();
		app->Run();
		CeLog::Log::Terminate();
		delete app;
	}
}

#endif

#endif //WORKSPACE_ENTRYPOINT_HPP
