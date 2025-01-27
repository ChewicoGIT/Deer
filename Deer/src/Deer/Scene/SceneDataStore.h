#pragma once
#include "Deer/DataStore/DataStore.h"
#include "Deer/DataStore/Path.h"
#include "Deer/Core/Core.h"

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

