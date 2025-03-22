#pragma once
#include "Deer/Application.h"
#include "DeerRender/KeyCodes.h"

namespace Deer {
	class Input {
	public:
		static bool isKeyPressed(unsigned int key);
		static bool isMouseButtonPressed(int button);
		static void getMousePos(float& x, float& y);
	};
}