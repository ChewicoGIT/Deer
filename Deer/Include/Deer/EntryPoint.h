#pragma once
#include "Deer/Core/Log.h"
#include "Deer/Application.h"
#include "Deer/ComponentScript.h"
#include "Deer/ScriptEngine.h"
#include "Deer/Scene/Entity.h"

//Temp
#include "DeerStudio/Project.h"

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
}

int main(int argc, char** argv) {
	return Deer::Main(argc, argv);
}