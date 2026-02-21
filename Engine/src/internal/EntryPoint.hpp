//
// Created by Catalin Chirosca on 2026-02-15.
//

#pragma once

#ifndef CE_ENTRYPOINT_HPP
#define CE_ENTRYPOINT_HPP

#ifdef CE_API

int main(const int argc, char* argv[]) {
	Log::Log::Init();
	{
		[[maybe_unused]] Time::Timer timer;
		const auto app = Core::CreateApplication(CMD::GetWindowProps(argc, argv));
		app->Run();
		delete app;
	}
	Log::Log::Terminate();
}

#endif

#endif //CE_ENTRYPOINT_HPP
