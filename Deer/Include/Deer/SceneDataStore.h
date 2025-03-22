#pragma once
#include "Deer/DataStore.h"
#include "Deer/Path.h"

namespace Deer {
	class Scene;

	namespace SceneDataStore {
		Scene loadScene(const Path& name);

		void deleteSceneJson(const Path& name);
		void exportSceneJson(Scene& scene, const Path& name);
		void exportScenesBin();

		void exportRuntimeScene(Scene& scene);
		Scene importRuntimeScene();
	}
}

