#pragma once
#include "Deer/Core/Timestep.h"

namespace Deer {
	class EditorPannel {
	public:
		virtual ~EditorPannel() = default;
		virtual void onUpdate(Timestep timestep) { }
		virtual void onImGui() { }
	};
}

