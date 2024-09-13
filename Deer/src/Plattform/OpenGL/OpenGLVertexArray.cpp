#include "OpenGLVertexArray.h"
#include "Plattform/OpenGL/OpenGLBuffer.h"
#include "Deer/Core/Log.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Deer {
	OpenGLVertexArray::OpenGLVertexArray() {
		glGenVertexArrays(1, &m_vertexArray);
	}

	OpenGLVertexArray::~OpenGLVertexArray() {
		glDeleteVertexArrays(1, &m_vertexArray);
	}

	void OpenGLVertexArray::bind() const {
		glBindVertexArray(m_vertexArray);
	}

	void OpenGLVertexArray::unbind() const {
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
		if (vertexBuffer->getLayout().getElements().size() == 0)
			DEER_CORE_ERROR("Vertex buffer has no layout");

		glBindVertexArray(m_vertexArray);
		vertexBuffer->bind();

		unsigned int index = 0;
		BufferLayout& layout = vertexBuffer->getLayout();
		for (auto& element : layout) {
			glEnableVertexAttribArray(index);

			switch (element.shaderType) {
			case ShaderDataType::None: DEER_CORE_ERROR("Shader datas type none not suported!"); break;
			case ShaderDataType::FloatingPoint:
				glVertexAttribPointer(index, dataTypeCount(element.type), getOpenGLDataType(element.type), GL_FALSE, layout.getStride(), (const void*)element.offset); break;
			case ShaderDataType::NormalizedFloatingPoint:
				glVertexAttribPointer(index, dataTypeCount(element.type), getOpenGLDataType(element.type), GL_TRUE, layout.getStride(), (const void*)element.offset); break;
			case ShaderDataType::Integer:
				glVertexAttribIPointer(index, dataTypeCount(element.type), getOpenGLDataType(element.type), layout.getStride(), (const void*)element.offset); break;
			}

			index++;
		}
		
		m_vertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
		glBindVertexArray(m_vertexArray);
		indexBuffer->bind();

		m_indexBuffer = indexBuffer;
	}
}

