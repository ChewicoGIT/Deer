#pragma once
#include "Deer/Application.h"
#include "Deer/Log.h"

extern Deer::Application* createApplication(int argc, char** argv);

namespace Deer {
	int Main(int argc, char** argv) {
		Core::argc = argc;
		Core::argv = argv;

		Log::init();
		DEER_CORE_TRACE("Initializing");

		Application* app = createApplication(argc, argv);

		int runResult = app->run();
		delete app;

		DEER_CORE_TRACE("Deinitializing");
		Log::shutdown();

		return runResult;
	}
}  // namespace Deer

int main(int argc, char** argv) { return Deer::Main(argc, argv); }