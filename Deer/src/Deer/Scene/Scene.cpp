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
}
