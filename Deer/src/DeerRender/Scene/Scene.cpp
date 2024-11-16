#include "Deer/Scene/Scene.h"
#include "Deer/Scene/Entity.h"
#include "Deer/Scene/Components.h"

namespace Deer {
	void Scene::render() {
		uid mainCamera = m_enviroment->tryGetMainCamera();
		if (mainCamera == 0)
			return;

		Entity& m_cameraEntity = m_enviroment->getEntity(mainCamera);
		SceneCamera sceneCamera;
		sceneCamera.camera = m_cameraEntity.getComponent<CameraComponent>();
		sceneCamera.transform = m_cameraEntity.getComponent<TransformComponent>();

		Scene::render(sceneCamera);
	}

	void Scene::render(SceneCamera sceneCamera) {
		m_enviroment->render(sceneCamera);
		m_voxelWorld->render(sceneCamera);
	}
}
