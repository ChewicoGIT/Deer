#pragma once
#include "Deer/Core/Application.h"
#include "Deer/EntryPoint.h"

namespace Deer {
	class DeerRuntimeApplication : public Deer::Application {
	public:
		DeerRuntimeApplication()
			: Application(Deer::WindowProps("Deer Runtime")) {
		}

		void onInit() override;
		void onShutdown() override;

		void onRender(Timestep delta) override;
		void onUpdate(Timestep delta) override;
	};
}

Deer::Application* createApplication(int argc, char** argv) {
	Deer::DeerRuntimeApplication* app = new Deer::DeerRuntimeApplication();
	return app;
}