#pragma once
#include "Deer/Core/Core.h"

namespace Deer {
	class ScriptEngine;
	class Scene;
	class SceneSerializer;

	class Project {
	public:
		static Ref<ScriptEngine> m_scriptEngine;

		static Ref<Scene> m_scene;
		static Ref<SceneSerializer> m_sceneSerializer;

		static void initializeBaseSystems();
		static void releaseBaseSystems();
	};
}

