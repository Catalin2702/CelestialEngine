//
// Module: Engine
// File: EntryPoint.hpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-15
// Updated: 2026-02-28
//

#pragma once

#ifndef CE_ENTRYPOINT_HPP
#define CE_ENTRYPOINT_HPP

#ifdef CE_API

int main(const int argc, char* argv[]) {
	Log::Log::Init();
	{
		[[maybe_unused]] Time::Chronometer timer;
		const auto app = Core::CreateApplication(CMD::GetWindowProps(argc, argv));
		app->Run();
		delete app;
	}
	Log::Log::Terminate();
}

#endif

#endif //CE_ENTRYPOINT_HPP
