#pragma once
#include "Deer/Core/Log.h"
#include "Deer/Core/Application.h"
#include "Deer/Scripting/ComponentScript.h"
#include "Deer/Scripting/ScriptEngine.h"
#include "Deer/Scene/Entity.h"

//Temp
#include "Deer/Core/Project.h"

#ifdef DEER_RENDER
#include "DeerRender/Render/RenderUtils.h"
#include "DeerRender/Render/RenderCommand.h"
#endif

extern Deer::Application* createApplication(int argc, char** argv);

namespace Deer {
	int Main(int argc, char** argv) {
		Core::argc = argc;
		Core::argv = argv;

		Log::init();
		DEER_CORE_TRACE("Initializing");

		Application* app = createApplication(argc, argv);

#ifdef DEER_RENDER
		RenderUtils::initializeRenderUtils();
		RenderCommand::init();
#endif

		Project::initializeBaseSystems();

		app->run();
		delete app;

		Project::releaseBaseSystems();

		DEER_CORE_TRACE("Deinitializing");
		Log::shutdown();

		return 0;
	}
}

int main(int argc, char** argv) {
	return Deer::Main(argc, argv);
}