//
// Created by Catalin Chirosca on 2026-02-15.
//

#pragma once

#ifndef WORKSPACE_ENTRYPOINT_HPP
#define WORKSPACE_ENTRYPOINT_HPP

#ifdef CE_API

namespace CeCore = CE::Core;
namespace CeLog = CE::Tools::Log;

extern CeCore::Application* CeCore::CreateApplication();

int main() {
	CeLog::Log::Init();
	CE_INFO("Hi var={0}", 5);
	const auto app = CeCore::CreateApplication();
	app->Run();
	delete app;
}

#endif

#endif //WORKSPACE_ENTRYPOINT_HPP
