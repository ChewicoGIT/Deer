#pragma once
#include "Deer/Core/Memory.h"
#include "Deer/Voxels/VoxelWorldProps.h"

#ifdef DEER_RENDER
#include "DeerRender/Scene/GizmoRenderer.h"
#endif

#include <vector>
#include <string>

namespace Deer {
	class VoxelWorld;
	class Environment;

	class Scene {
	public:
		Scene();
		~Scene();

		void createVoxelWorld(const VoxelWorldProps&);
		void deleteVoxelWorld();
		void clear();

		void beginExecution();
		void updateInternalVars();
		void endExecution();
	public:
		inline Ref<Environment>& getMainEnviroment() { return m_enviroment; }
		inline Ref<VoxelWorld>& getVoxelWorld() { return m_voxelWorld; }
		inline bool getExecutingState() { return m_isExecuting; }
	private:
		Ref<Environment> m_enviroment;
		Ref<VoxelWorld> m_voxelWorld;

		bool m_isExecuting = false;
#ifdef DEER_RENDER
	public:
		void render();
		void render(SceneCamera);
		inline GizmoRenderer& getMainGizmoRenderer() { return m_gizmoRenderer; }
	private:
		GizmoRenderer m_gizmoRenderer;
#endif
	};
}

