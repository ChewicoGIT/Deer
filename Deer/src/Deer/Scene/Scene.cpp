#include "Scene.h"
#include "Deer/Core/Log.h"

namespace Deer {
	Scene::Scene() {
		DEER_CORE_TRACE("Creating scene");
		m_enviroment = Ref<Environment>(new Environment("Scene Root"));
	}
	Scene::~Scene() {
		DEER_CORE_TRACE("Destroying scene");
	}

	void Scene::render() {
		uid mainCamera = m_enviroment->tryGetMainCamera();
		if (mainCamera == 0)
			return;

		Entity& m_cameraEntity = m_enviroment->getEntity(mainCamera);
		m_enviroment->render(m_cameraEntity);
	}

	void Scene::clear() {
		m_enviroment->clear();
	}
}
