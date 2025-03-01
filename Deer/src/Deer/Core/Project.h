#pragma once
#include "Deer/Core/Core.h"

namespace Deer {
	class ScriptEngine;
	class Scene;

	class Project {
	public:
		static Ref<ScriptEngine> m_scriptEngine;

		static Scene m_scene;

		static void initializeBaseSystems();
		static void releaseBaseSystems();
	};
}

