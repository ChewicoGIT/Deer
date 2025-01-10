#pragma once
#include "Deer/DataStore/DataStore.h"
#include "Deer/DataStore/Path.h"
#include "Deer/Core/Core.h"

namespace Deer {
	class Scene;

	class SceneDataStore {
	public:
		SceneDataStore(DataStore& dataStore) : m_dataStore(dataStore) { }

		void saveBin(Ref<Scene>& scene, const Path& path);
		Ref<Scene> loadBin(const Path& path);

	private:
		DataStore& m_dataStore;
	};
}

