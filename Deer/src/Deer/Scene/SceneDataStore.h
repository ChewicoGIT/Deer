#pragma once
#include "Deer/DataStore/DataStore.h"
#include "Deer/DataStore/Path.h"
#include "Deer/Core/Core.h"

namespace Deer {
	class Scene;

	namespace SceneDataStore {

		void saveBin(Ref<Scene>& scene, const Path& path);
		Ref<Scene> loadBin(const Path& path);
	}
}

