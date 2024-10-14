#include "EnvironmentSerializer.h"
#include "cereal/archives/json.hpp"

#include <fstream>

namespace Deer {
	EnvironmentSerializer::EnvironmentSerializer(Ref<Environment>& environment) 
		: m_environment(environment){
	}

	void EnvironmentSerializer::serialize(const std::string& filePath) {

		//std::ofstream file(filePath);
		//cereal::JSONOutputArchive archive(file); // depending on the archive type, data may be
		//// output to the stream as it is serialized, or
		//// only on destruction
		//archive(*m_environment);
	}

	void EnvironmentSerializer::deserialize(const std::string& filePath) {
	}
}
