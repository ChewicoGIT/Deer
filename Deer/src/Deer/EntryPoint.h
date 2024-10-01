#pragma once
#include "Deer/Core/Application.h"
#include "Deer/Core/Log.h"
#include "Deer/Render/RenderUtils.h"

extern Deer::Application* createApplication(int argc, char** argv);

namespace Deer {
	int Main(int argc, char** argv) {
		Log::init();
		DEER_CORE_TRACE("Creating app");

		Application* app = createApplication(argc, argv);
		RenderUtils::initializeRenderUtils();

		app->run();
		delete app;

		DEER_CORE_TRACE("Shuting app");
		Log::shutdown();
		return 0;
	}
}

int main(int argc, char** argv) {
	return Deer::Main(argc, argv);
}