#pragma once
#include "Deer/Core/Application.h"
#include "Deer/Core/Log.h"
#include "Deer/Render/RenderUtils.h"
#include "Deer/Scripting/ComponentScript.h"
#include "Deer/Scripting/ScriptEngine.h"
#include "Deer/Scene/Entity.h"

//Temp
#include "Deer/Core/Project.h"

extern Deer::Application* createApplication(int argc, char** argv);

namespace Deer {
	int Main(int argc, char** argv) {
		Log::init();
		DEER_CORE_TRACE("Creating app");

		Application* app = createApplication(argc, argv);

		RenderUtils::initializeRenderUtils();
		Project::initializeBaseSystems();

		Project::m_scriptEngine->initScriptEngine(std::filesystem::path("scripts"));

		app->run();
		delete app;

		Project::m_scriptEngine->shutdownScriptEngine();
		Project::releaseBaseSystems();

		DEER_CORE_TRACE("Shuting app");
		Log::shutdown();
		return 0;
	}
}

int main(int argc, char** argv) {
	return Deer::Main(argc, argv);
}