#pragma once
#include "Deer/DataStore/DataStore.h"
#include "Deer/DataStore/Path.h"
#include "Deer/Core/Core.h"

namespace Deer {
	class Scene;

	namespace SceneDataStore {
		Ref<Scene> loadScene(const Path& name);

		void exportSceneJson(Ref<Scene>& scene, const Path& name);
		void exportScenesBin();

		void exportRuntimeScene(Ref<Scene>& scene);
		Ref<Scene> importRuntimeScene();
	}
}

