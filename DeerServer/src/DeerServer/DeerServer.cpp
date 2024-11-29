#include "DeerServer.h"

#include "Deer/Core/Project.h"
#include "Deer/Scripting/ScriptEngine.h"
#include "Deer/Scene/Scene.h"

namespace Deer {
	void DeerServerApplication::onInit() {
		Project::m_scriptEngine->compileScriptEngine(std::filesystem::path("scripts"));
		Project::m_scriptEngine->beginExecutionContext();
		Project::m_scene->beginExecution();
	}

	void DeerServerApplication::onShutdown() {
		if (Project::m_scene->getExecutingState())
			Project::m_scene->endExecution();

		Project::m_scriptEngine->shutdownScriptEngine();
	}

	void DeerServerApplication::onUpdate(Timestep delta) {
		Project::m_scene->updateExecution();
	}
}