#pragma once
#include "Deer/EntryPoint.h"
#include "Deer/Core/Application.h"
#include "Deer/Core/Events/Event.h"

#include "DeerStudio/Editor/ActiveEntity.h"
#include "DeerStudio/Editor/EditorPannel.h"

namespace Deer {
	class DeerStudioApplication : public Deer::Application {
	public:
		DeerStudioApplication()
			: Application(Deer::WindowProps("Deer Studio")) { }
	private:
		int originEntity, dirEntity, hitEntity;
	private:
		void onInit() override;
		void onShutdown() override;

		void onRender(Timestep delta) override;
		void onUpdate(Timestep delta) override;

		void onEvent(Event& e) override;
		void onImGUI() override;
		void drawMenuBar();
		void onChangeScene();
		
		Ref<ActiveEntity> m_activeEntity;
		std::vector<Ref<EditorPannel>> pannels;
	};

}

Deer::Application* createApplication(int argc, char** argv) {
	Deer::DeerStudioApplication* app = new Deer::DeerStudioApplication();
	return app;
}