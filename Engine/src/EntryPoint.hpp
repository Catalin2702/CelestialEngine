//
// Created by Catalin Chirosca on 2026-02-15.
//

#ifndef WORKSPACE_ENTRYPOINT_HPP
#define WORKSPACE_ENTRYPOINT_HPP

#ifdef CE_API

namespace CeCore = CE::Core;

extern CeCore::Application* CeCore::CreateApplication();

int main() {
	auto app = CeCore::CreateApplication();
	app->Run();
	delete app;
}

#endif

#endif //WORKSPACE_ENTRYPOINT_HPP
