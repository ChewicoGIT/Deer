#include "OpenGLContext.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Deer/Core/Log.h"

namespace Deer {
	OpenGLContext::OpenGLContext(GLFWwindow* window)
		: m_window(window) {
	}

	void OpenGLContext::init() { 
		glfwMakeContextCurrent(m_window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		if (status == 0) {
			DEER_CORE_ERROR("Failed to load glad in OpenGL render context");
		}

		DEER_CORE_TRACE("OpenGL driver: {0}", (char*)glGetString(GL_VENDOR));
		DEER_CORE_TRACE("Initialized glad");
	}

	void OpenGLContext::swapBuffers() {
		glfwSwapBuffers(m_window);
	}
}