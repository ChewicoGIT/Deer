#pragma once
#include "DeerRender/Core/Events/Event.h"

namespace Deer {
	extern float viewport_relativeXMouse, viewport_relativeYMouse;
	extern bool viewport_isActive;

	void viewport_onImGui();
	void viewport_onEvent(Event&);
}