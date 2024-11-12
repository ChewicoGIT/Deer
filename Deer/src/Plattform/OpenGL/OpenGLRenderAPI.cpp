#include "OpenGLRenderAPI.h"
#include "Plattform/OpenGL/OpenGLBuffer.h"
#include "DeerRender/Render/VertexArray.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

namespace Deer {
	void OpenGLRenderAPI::init() {
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	}

	void OpenGLRenderAPI::clearColor(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRenderAPI::clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRenderAPI::drawIndex(const Ref<VertexArray>& vertexArray) {
		glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), getOpenGLIndexDataType(vertexArray->getIndexBuffer()->getIndexDataType()), nullptr);
	}

	void OpenGLRenderAPI::drawLines(const Ref<VertexArray>& vertexArray) {
		glDrawElements(GL_LINES, vertexArray->getIndexBuffer()->getCount(), getOpenGLIndexDataType(vertexArray->getIndexBuffer()->getIndexDataType()), nullptr);
	}
}