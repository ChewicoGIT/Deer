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

		void beginExecution();
		void updateExecution();
		void endExecution();
#ifndef DEER_SERVICE
		void render();
#endif

		void clear();

		inline Ref<Environment>& getMainEnviroment() { return m_enviroment; }
		inline bool getExecutingState() { return m_isExecuting; }
	private:
		Ref<Environment> m_enviroment;
		bool m_isExecuting = false;
	};
}

