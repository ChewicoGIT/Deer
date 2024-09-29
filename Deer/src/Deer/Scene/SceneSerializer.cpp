#include "SceneSerializer.h"
#include "cereal/archives/json.hpp"
#include "Deer/Scene/Serialization.h"

#include <fstream>

namespace Deer {
	void SceneSerializer::serialize(const std::string& filePath) {
		std::ofstream file(filePath);
		cereal::JSONOutputArchive archive(file);
		m_currentScenePath = filePath;

		archive(cereal::make_nvp("scene", m_scene));
	}

	void SceneSerializer::deserialize(const std::string& filePath) {
		std::ifstream file(filePath);
		cereal::JSONInputArchive archive(file);

		m_scene->getMainEnviroment()->clear();
		m_currentScenePath = filePath;

		archive(cereal::make_nvp("scene", m_scene));
	}
}
