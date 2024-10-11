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

		void execute();
		void updateScene();
		void stop();

		void render();
		void clear();

		inline Ref<Environment>& getMainEnviroment() { return m_enviroment; }
		inline bool getExecutingState() { return m_isExecuting; }
	private:
		Ref<Environment> m_enviroment;
		bool m_isExecuting = false;
	};
}

