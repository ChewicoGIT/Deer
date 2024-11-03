#include "RuntimeLayer.h"
#include "Deer/Core/Project.h"

#include "Deer/Scripting/ScriptEngine.h"
#include "Deer/Scene/Scene.h"

namespace Deer {
	void Deer::RuntimeLayer::onAttach() {
		Project::m_scriptEngine->beginExecutionContext();
		Project::m_scene->beginExecution();
	}

	void RuntimeLayer::onRender(Timestep delta) {
		Project::m_scene->render();
	}

	void RuntimeLayer::onUpdate(Timestep delta) {
		Project::m_scene->updateExecution();
	}
}
