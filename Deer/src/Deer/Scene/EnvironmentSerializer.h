#pragma once
#include "Deer/Core/Core.h"
#include "Deer/Scene/Enviroment.h"

#include <string>

namespace Deer {
	class EnvironmentSerializer {
	public:
		EnvironmentSerializer(Ref<Environment>& environment);

		void serialize(const std::string& filePath);
		void deserialize(const std::string& filePath);

	private:
		Ref<Environment> m_environment;
	};
}

