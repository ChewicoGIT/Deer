#pragma once
#include "Deer/Core/Core.h"
#include "Enviroment.h"
#include <vector>
#include <string>

namespace Deer {
	class Scene {
	public:
		Scene();
		~Scene();

		inline Ref<Environment>& getMainEnviroment() { return m_enviroment; }
	private:
		Ref<Environment> m_enviroment;
		// Chunk world -- saved in a bin file

	};
}

