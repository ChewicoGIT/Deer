#pragma once
#include "DeerRender/Events/Event.h"
#include "DeerRender/Scene/SceneCamera.h"

namespace Deer {
	extern float viewport_relativeXMouse, viewport_relativeYMouse;
	extern bool viewport_isActive;
	extern SceneCamera viewport_sceneCamera;

	void viewport_onImGui();
	void viewport_onEvent(Event&);

	inline bool viewportIsActive() {
		return viewport_relativeXMouse >= 0 && viewport_relativeXMouse <= 1 && viewport_relativeYMouse >= 0 && viewport_relativeYMouse <= 1;
	}
}