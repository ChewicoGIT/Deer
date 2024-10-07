#pragma once
#include "Deer/Core/Application.h"
#include "Deer/Core/Log.h"
#include "Deer/Render/RenderUtils.h"
#include "Deer/Scripting/RoeScript.h"

//Temp
#include "Deer/Core/Project.h"

extern Deer::Application* createApplication(int argc, char** argv);

namespace Deer {
	int Main(int argc, char** argv) {
		Log::init();
		DEER_CORE_TRACE("Creating app");

		Project::m_scriptEngine.initScriptEngine();
		Project::m_scriptEngine.loadRoeModule(std::filesystem::path("roe_scripts"));
		Project::m_scriptEngine.createScriptInstance(0);
		Project::m_scriptEngine.updateRoeInstance(0);

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