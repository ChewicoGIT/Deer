#include "OpenGLVertexArray.h"
#include "Plattform/OpenGL/OpenGLBuffer.h"
#include "Deer/Core/Log.h"
#include "objload.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Deer {
	Ref<VertexArray> VertexArray::create() {
		return Ref<VertexArray>(new OpenGLVertexArray());
	}

	Ref<VertexArray> VertexArray::create(const std::string& filePath) {
		return Ref<VertexArray>(new OpenGLVertexArray(filePath));
	}

	OpenGLVertexArray::OpenGLVertexArray() {
		glGenVertexArrays(1, &m_vertexArray);
	}

	OpenGLVertexArray::OpenGLVertexArray(const std::string& filePath) {
		Ref<VertexBuffer> m_vertexBuffer;
		Ref<VertexBuffer> m_uvBuffer;
		Ref<VertexBuffer> m_normalBuffer;
		Ref<IndexBuffer> m_indexBuffer;

		obj::Model model = obj::loadModelFromFile(filePath);
		std::vector<unsigned short>& triangles = model.faces.at("default");

		glGenVertexArrays(1, &m_vertexArray);

		m_vertexBuffer = VertexBuffer::create(&model.vertex[0], model.vertex.size() * sizeof(float));

		BufferLayout vertexbufferLayout({
			{"a_Position", DataType::Float3, ShaderDataType::FloatingPoint }
		});

		m_vertexBuffer->setLayout(vertexbufferLayout);
		addVertexBuffer(m_vertexBuffer);

		m_uvBuffer = VertexBuffer::create(&model.texCoord[0], model.texCoord.size() * sizeof(float));

		BufferLayout uvbufferLayout({
			{"a_uv", DataType::Float2, ShaderDataType::FloatingPoint }
			});


		m_uvBuffer->setLayout(uvbufferLayout);
		addVertexBuffer(m_uvBuffer);

		m_normalBuffer = VertexBuffer::create(&model.normal[0], model.normal.size() * sizeof(float));

		BufferLayout normalBufferLayout({
			{"a_normal", DataType::Float3, ShaderDataType::NormalizedFloatingPoint }
			});


		m_normalBuffer->setLayout(normalBufferLayout);
		addVertexBuffer(m_normalBuffer);

		m_indexBuffer = IndexBuffer::create(&triangles[0], triangles.size() * sizeof(unsigned short), IndexDataType::Unsigned_Short);
		setIndexBuffer(m_indexBuffer);
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

