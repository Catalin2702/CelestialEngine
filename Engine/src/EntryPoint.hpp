//
// Created by Catalin Chirosca on 2026-02-15.
//

#ifndef WORKSPACE_ENTRYPOINT_HPP
#define WORKSPACE_ENTRYPOINT_HPP

#ifdef CE_API

extern CE::Core::Application* CE::Core::CreateApplication();

int main() {
	auto app = CE::Core::CreateApplication();
	app->Run();
	delete app;
}

#endif

#endif //WORKSPACE_ENTRYPOINT_HPP
