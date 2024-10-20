#include "SceneSerializer.h"
#include "cereal/archives/json.hpp"
#include "cereal/archives/binary.hpp"
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

	void SceneSerializer::serializeBinary(const std::string& filePath) {
		std::ofstream file(filePath);
		cereal::BinaryOutputArchive archive(file);
		m_currentScenePath = filePath;

		archive(cereal::make_nvp("scene", m_scene));
	}

	void SceneSerializer::deserializeBinary(const std::string& filePath) {
		std::ifstream file(filePath);
		cereal::BinaryInputArchive archive(file);

		m_scene->getMainEnviroment()->clear();
		m_currentScenePath = filePath;

		archive(cereal::make_nvp("scene", m_scene));
	}
}
