#include "Input.h"

namespace Deer {
	bool Input::isKeyPressed(unsigned int key) {
		return Application::s_application->m_window->getKeyPressed(key);
	}

	bool Input::isMouseButtonPressed(int button) {
		return Application::s_application->m_window->getMouseButton(button);
	}

	void Input::getMousePos(float& x, float& y) {
		return Application::s_application->m_window->getMousePos(x, y);
	}
}