#pragma once
#include "Deer/Core/Application.h"
#include "Deer/EntryPoint.h"

namespace Deer {
	class DeerServerApplication : public Deer::Application {
	public:
		DeerServerApplication()
			: Application() {
		}

		void onInit() override;
		void onShutdown() override;

		void onUpdate(Timestep delta) override;
	};
}

Deer::Application* createApplication(int argc, char** argv) {
	Deer::DeerServerApplication* app = new Deer::DeerServerApplication();
	return app;
}