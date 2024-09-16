#pragma once
#include "Deer/Core/Timestep.h"

namespace Deer {

	class Event;

	class EditorPannel {
	public:
		virtual ~EditorPannel() = default;
		virtual void onUpdate(Timestep timestep) { }
		virtual void onEvent(Event& e) { }
		virtual void onImGui() { }
	};
}

