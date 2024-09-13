#include "Deer/Core/Application.h"
#include "Deer/EntryPoint.h"
#include "Deer/ImGui/ImGuiLayer.h"
#include "Deer/Core/Window.h"

#include "DeerStudio/DeerStudioLayer.h"

#include <iostream>

class DeerStudioApplication : public Deer::Application {
public:
	DeerStudioApplication() 
	: Application(Deer::WindowProps("Deer Studio")) {

	}

	virtual void onInit() override {
		pushLayer(new Deer::DeerStudioLayer());
	}
};

Deer::Application* createApplication(int argc, char** argv) {
	DeerStudioApplication* app = new DeerStudioApplication();
	return app;
}