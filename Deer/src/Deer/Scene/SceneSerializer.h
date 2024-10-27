#pragma once
#include "Scene.h"
#include <string>

namespace Deer {
	class SceneSerializer {
	public:
		SceneSerializer(Ref<Scene>& scene) : m_scene(scene), m_currentScenePath("_NO_INITIALIZED_") { }

		void serialize(const std::string& filePath);
		void deserialize(const std::string& filePath);

		void serializeBinary(const std::string& filePath);
		void deserializeBinary(const std::string& filePath);

		const std::string& getCurrentScenePath() { return m_currentScenePath; }
		inline bool getSceneExecutingState() { return m_scene->getExecutingState(); }
	private:
		std::string m_currentScenePath;
		Ref<Scene> m_scene;
	};
}

