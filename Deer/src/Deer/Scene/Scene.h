#pragma once
#include "Deer/Core/Core.h"
#include "Deer/Voxels/VoxelWorld.h"
#include "Deer/Scene/Enviroment.h"

#include <vector>
#include <string>

namespace Deer {
	class Scene {
	public:
		Scene();
		~Scene();

		void createVoxelWorld(const VoxelWorldProps&);
		void clear();

		void beginExecution();
		void updateExecution();
		void endExecution();
#ifdef DEER_RENDER
		void render();
		void render(SceneCamera);
#endif

		inline Ref<Environment>& getMainEnviroment() { return m_enviroment; }
		inline Ref<VoxelWorld>& getVoxelWorld() { return m_voxelWorld; }
		inline bool getExecutingState() { return m_isExecuting; }
	private:
		Ref<Environment> m_enviroment;
		Ref<VoxelWorld> m_voxelWorld;

		bool m_isExecuting = false;
	};
}

