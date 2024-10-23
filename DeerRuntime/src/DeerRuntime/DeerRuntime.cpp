#include "Deer/Core/Application.h"
#include "Deer/EntryPoint.h"
#include "Deer/Core/Window.h"

#include "DeerRuntime/RuntimeLayer.h"

class DeerRuntimeApplication : public Deer::Application {
public:
	DeerRuntimeApplication()
		: Application(Deer::WindowProps("Deer Studio")) {

	}

	virtual void onInit() override {
		pushLayer(new Deer::RuntimeLayer());
	}
};

Deer::Application* createApplication(int argc, char** argv) {
	DeerRuntimeApplication* app = new DeerRuntimeApplication();
	return app;
}