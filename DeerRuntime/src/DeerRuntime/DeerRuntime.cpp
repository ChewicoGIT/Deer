#include "DeerRuntime.h"

#include "Deer/Core/Project.h"
#include "Deer/Scripting/ScriptEngine.h"
#include "Deer/Scene/Scene.h"

namespace Deer {
	void DeerRuntimeApplication::onInit() {
		Project::m_scriptEngine->compileScriptEngine(std::filesystem::path("scripts"));
		Project::m_scriptEngine->beginExecutionContext();
		Project::m_scene->beginExecution();
	}

	void DeerRuntimeApplication::onShutdown() {
		if (Project::m_scene->getExecutingState())
			Project::m_scene->endExecution();

		Project::m_scriptEngine->shutdownScriptEngine();
	}

	void DeerRuntimeApplication::onRender(Timestep delta) {
		uid mainCam = Project::m_scene->getMainEnviroment()->tryGetMainCamera();
		if (mainCam == 0)
			return;
		Entity& cam = Project::m_scene->getMainEnviroment()->getEntity(mainCam);

		float witdth = Application::s_application->m_window->getWitdth();
		float height = Application::s_application->m_window->getHeight();

		cam.getComponent<CameraComponent>().aspect = witdth / height;

		Project::m_scene->render();
	}

	void DeerRuntimeApplication::onUpdate(Timestep delta) {
		Project::m_scene->updateInternalVars();
	}
}