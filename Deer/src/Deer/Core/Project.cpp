#include "Project.h"

#include "Deer/Asset/AssetManager.h"
#include "Deer/Scripting/ScriptEngine.h"

#include "Deer/Scene/Scene.h"
#include "Deer/Scene/SceneSerializer.h"

#include "Deer/DataStore/DataStore.h"

namespace Deer {
	Ref<ScriptEngine> Project::m_scriptEngine;

	Ref<Scene> Project::m_scene;
	Ref<SceneSerializer> Project::m_sceneSerializer;

	void Project::initializeBaseSystems() {
		m_scriptEngine = Ref<ScriptEngine>(new ScriptEngine());

		m_scene = Ref<Scene>(new Scene());
		m_sceneSerializer = Ref<SceneSerializer>(new SceneSerializer(Project::m_scene));

	}

	void Project::releaseBaseSystems() {
		m_scriptEngine.reset();
		m_scene.reset();
		m_sceneSerializer.reset();
	}
}