#pragma once
#include "Scene.h"
#include <string>
#include <functional>

namespace Deer {
	class SceneSerializer {
	public:
		// In case DEER_SERVICE is defined by default serverSerialization will be true allewaysk9iy88ugyu 
  		SceneSerializer(Ref<Scene>& scene) : m_scene(scene), m_currentScenePath("_NO_INITIALIZED_") { }
		
		void serialize(const std::string& filePath, bool serverSerialization = false);
		void deserialize(const std::string& filePath, bool serverSerialization = false);

		void serializeBinary(const std::string& filePath, bool serverSerialization = false);
		void deserializeBinary(const std::string& filePath, bool serverSerialization = false);

		void setSceneChangeCallback(std::function<void()> func) { m_sceneChangeCallback = func; }

		const std::string& getCurrentScenePath() { return m_currentScenePath; }
		inline bool getSceneExecutingState() { return m_scene->getExecutingState(); }
	private:
		std::string m_currentScenePath;
		std::function<void()> m_sceneChangeCallback;
		Ref<Scene> m_scene;
	};
}

