#pragma once
#include "Deer/Core/Application.h"
#include "Deer/Core/KeyCodes.h"

namespace Deer {
	class Input {
	public:
		static bool isKeyPressed(unsigned int key);
		static bool isMouseButtonPressed(int button);
		static void getMousePos(float& x, float& y);
	};
}