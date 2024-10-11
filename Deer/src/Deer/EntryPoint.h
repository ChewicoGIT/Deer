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

		Project::initializeBaseSystems();

		Project::m_scriptEngine->initScriptEngine();
		Project::m_scriptEngine->loadScripts(std::filesystem::path("scripts"));

		Project::m_scriptEngine->beginExecutionContext();

		for (auto& script : Project::m_scriptEngine->getComponentScripts()) {
			Ref<ComponentScriptInstance> componentScript = Project::m_scriptEngine->createComponentScriptInstance(
				script.first, Entity::nullEntity);

			DEER_CORE_INFO("Executing {0}", script.first);
			componentScript->update();
		}

		Project::m_scriptEngine->endExecutionContext();

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