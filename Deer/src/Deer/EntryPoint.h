#pragma once
#include "Deer/Core/Application.h"
#include "Deer/Core/Log.h"
#include "Deer/Render/RenderUtils.h"
#include "Deer/Scripting/DeerScript.h"

//Temp
#include "Deer/Core/Project.h"

extern Deer::Application* createApplication(int argc, char** argv);

namespace Deer {
	int Main(int argc, char** argv) {
		Log::init();
		DEER_CORE_TRACE("Creating app");

		Project::m_scriptEngine.initScriptEngine();
		Project::m_scriptEngine.loadScripts(std::filesystem::path("scripts"));

		Project::m_scriptEngine.beginExecutionContext();

		Project::m_scriptEngine.createScriptInstance(0);
		Project::m_scriptEngine.updateRoeInstance(0);

		Project::m_scriptEngine.endExecutionContext();

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