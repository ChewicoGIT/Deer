#pragma once
#include "Deer/Render/RenderContext.h"

struct GLFWwindow;

namespace Deer {
	class OpenGLContext : public RenderContext {
	public:
		OpenGLContext(GLFWwindow* window);

		void init() override;
		void swapBuffers() override;
	private:
		GLFWwindow* m_window;
	};
}

