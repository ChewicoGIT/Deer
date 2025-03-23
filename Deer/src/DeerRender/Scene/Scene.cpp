#include "Deer/Scene.h"
#include "Deer/Enviroment.h"
#include "Deer/Components.h"
#include "Deer/VoxelWorld.h"
#include "DeerRender/Render/RenderCommand.h"
#include "Deer/Enviroment.h"

namespace Deer {
	void Scene::render() {
		uint32_t mainCamera = m_enviroment->tryGetMainCamera();
		if (mainCamera == 0)
			return;

		Entity& m_cameraEntity = m_enviroment->getEntity(mainCamera);
		SceneCamera sceneCamera;
		sceneCamera.camera = m_cameraEntity.getComponent<CameraComponent>();
		sceneCamera.transform = m_cameraEntity.getComponent<TransformComponent>();

		Scene::render(sceneCamera);
	}

	void Scene::render(SceneCamera sceneCamera) {
		RenderCommand::setDepthBuffer(true);
		m_enviroment->render(sceneCamera);
		if (m_voxelWorld)
			m_voxelWorld->render(sceneCamera);
		RenderCommand::setDepthBuffer(false);
		m_gizmoRenderer.render(sceneCamera);
	}

}
