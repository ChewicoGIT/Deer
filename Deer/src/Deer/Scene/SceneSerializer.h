#pragma once
#include "Scene.h"
#include <string>

namespace Deer {
	class SceneSerializer {
	public:
		SceneSerializer(Ref<Scene>& scene) : m_scene(scene) { }

		void serialize(const std::string& filePath);
		void deserialize(const std::string& filePath);

	private:
		Ref<Scene> m_scene;
	};
}

