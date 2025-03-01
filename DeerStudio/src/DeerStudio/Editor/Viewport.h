#pragma once
#include "DeerRender/Core/Events/Event.h"
#include "DeerRender/Scene/SceneCamera.h"

namespace Deer {
	extern float viewport_relativeXMouse, viewport_relativeYMouse;
	extern bool viewport_isActive;
	extern bool viewport_hasClicked;
	extern SceneCamera viewport_sceneCamera;

	void viewport_onImGui();
	void viewport_onEvent(Event&);
}