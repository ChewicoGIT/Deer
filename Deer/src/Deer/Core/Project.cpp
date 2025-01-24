#include "Project.h"

#include "Deer/Asset/AssetManager.h"
#include "Deer/Scripting/ScriptEngine.h"

#include "Deer/Scene/Scene.h"

#include "Deer/DataStore/DataStore.h"

namespace Deer {
	Ref<ScriptEngine> Project::m_scriptEngine;

	Scene Project::m_scene;

	void Project::initializeBaseSystems() {
		m_scriptEngine = Ref<ScriptEngine>(new ScriptEngine());

	}

	void Project::releaseBaseSystems() {
		m_scriptEngine.reset();
	}
}