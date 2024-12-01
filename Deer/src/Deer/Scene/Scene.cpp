#include "Scene.h"
#include "Deer/Core/Log.h"
#include "Deer/Core/Project.h"
#include "Deer/Scripting/ScriptEngine.h"
#include "Deer/Scene/Components.h"

namespace Deer {
	Scene::Scene() {
		DEER_CORE_TRACE("Creating scene");
		m_enviroment = Ref<Environment>(new Environment("Scene Root"));
	}

	Scene::~Scene() {
		DEER_CORE_TRACE("Destroying scene");
	}

	void Scene::beginExecution() {
		DEER_CORE_ASSERT(!m_isExecuting, "Deer scene is already executing");
		m_isExecuting = true;

		DEER_CORE_INFO("Executing Scene...");
		Project::m_scriptEngine->beginExecutionContext();

		// Instantiate all the scripts
		auto view = m_enviroment->m_registry.view<ScriptComponent, TagComponent>();
		for (auto& entID : view) {
			auto& tagComponent = view.get<TagComponent>(entID);
			auto& componentScript = view.get<ScriptComponent>(entID);

			Entity& entity = m_enviroment->getEntity(tagComponent.entityUID);
			componentScript.roeInstance = Project::m_scriptEngine->createComponentScriptInstance(componentScript.scriptID, entity);
		
			componentScript.roeInstance->start();
		}
	}

	void Scene::updateExecution() {
		// Update all scripts
		auto view = m_enviroment->m_registry.view<ScriptComponent>();
		for (auto& entID : view) {
			auto& componentScript = view.get<ScriptComponent>(entID);
			componentScript.roeInstance->updateExecution();
		}
	}

	void Scene::endExecution() {
		DEER_CORE_ASSERT(m_isExecuting, "Deer scene is not executing");
		m_isExecuting = false;

		// Deatach all scripts
		auto view = m_enviroment->m_registry.view<ScriptComponent>();
		for (auto& entID : view) {
			auto& componentScript = view.get<ScriptComponent>(entID);
			componentScript.roeInstance.reset();
		}

		DEER_CORE_INFO("Stoping Scene...");
	}

	void Scene::createVoxelWorld(const VoxelWorldProps& props) {
		m_voxelWorld = Ref<VoxelWorld>(new VoxelWorld(props));
	}

	void Scene::clear() {
		m_enviroment->clear();
		m_voxelWorld.reset();
	}
}
