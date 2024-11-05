#include "OpenGLBuffer.h"

#include "Deer/Core/Log.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Deer {

	////////////////////////////////////////////////////////////////////////
	// Vertex Buffer ///////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////
	Ref<VertexBuffer> VertexBuffer::create(void* data, unsigned int size) {
		return Ref<VertexBuffer>(new OpenGLVertexBuffer(data, size));
	}

	Ref<IndexBuffer> IndexBuffer::create(void* data, unsigned int size, IndexDataType indexDataType) {
		return Ref<IndexBuffer>(new OpenGLIndexBuffer(data, size, indexDataType));
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(void* data, unsigned int size) {
		glGenBuffers(1, &m_bufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);

		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		glDeleteBuffers(1, &m_bufferID);
	}

	void OpenGLVertexBuffer::bind() const {
		glBindBuffer(GL_VERTEX_ARRAY, m_bufferID);
	}

	void OpenGLVertexBuffer::setLayout(BufferLayout& layout) {
		m_bufferLayout = layout;
	}

	////////////////////////////////////////////////////////////////////////
	// Index Buffer ////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(void* data, unsigned int size, IndexDataType indexDataType)
		: m_indexDataType(indexDataType){
		glGenBuffers(1, &m_bufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		
		m_count = size / indexDataTypeSize(m_indexDataType);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer() {
		glDeleteBuffers(1, &m_bufferID);
	}

	void OpenGLIndexBuffer::bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);

	}

	unsigned int getOpenGLDataType(DataType type) {
		switch (type) {
		case DataType::None: DEER_CORE_ERROR("Unkown shader data type"); return 0;
			// Halfs : 2
		case DataType::Half:	return GL_HALF_FLOAT;
		case DataType::Half2:	return GL_HALF_FLOAT;
		case DataType::Half3:	return GL_HALF_FLOAT;
		case DataType::Half4:	return GL_HALF_FLOAT;
			// Floats : 4
		case DataType::Float: return  GL_FLOAT;
		case DataType::Float2: return GL_FLOAT;
		case DataType::Float3: return GL_FLOAT;
		case DataType::Float4: return GL_FLOAT;
			// Byte : 1
		case DataType::Byte:	return GL_BYTE;
		case DataType::Byte2:	return GL_BYTE;
		case DataType::Byte3:	return GL_BYTE;
		case DataType::Byte4:	return GL_BYTE;
			// Short : 2
		case DataType::Short:  return GL_SHORT;
		case DataType::Short2: return GL_SHORT;
		case DataType::Short3: return GL_SHORT;
		case DataType::Short4: return GL_SHORT;
			// Ints : 4
		case DataType::Int:  return GL_INT;
		case DataType::Int2: return GL_INT;
		case DataType::Int3: return GL_INT;
		case DataType::Int4: return GL_INT;
			// Byte : 1
		case DataType::Unsigned_Byte:  return GL_UNSIGNED_BYTE;
		case DataType::Unsigned_Byte2: return GL_UNSIGNED_BYTE;
		case DataType::Unsigned_Byte3: return GL_UNSIGNED_BYTE;
		case DataType::Unsigned_Byte4: return GL_UNSIGNED_BYTE;
			// Short : 2
		case DataType::Unsigned_Short:  return GL_UNSIGNED_SHORT;
		case DataType::Unsigned_Short2: return GL_UNSIGNED_SHORT;
		case DataType::Unsigned_Short3: return GL_UNSIGNED_SHORT;
		case DataType::Unsigned_Short4: return GL_UNSIGNED_SHORT;
			// Ints : 4
		case DataType::Unsigned_Int:	return GL_UNSIGNED_BYTE;
		case DataType::Unsigned_Int2:	return GL_UNSIGNED_BYTE;
		case DataType::Unsigned_Int3:	return GL_UNSIGNED_BYTE;
		case DataType::Unsigned_Int4: return   GL_UNSIGNED_BYTE;
		}

		DEER_CORE_ERROR("Unkown shader data type");
	}

	unsigned int getOpenGLIndexDataType(IndexDataType type) {
		switch (type) {
		case IndexDataType::None: DEER_CORE_ERROR("Unkown shader index data type"); return 0;
		case IndexDataType::Unsigned_Byte: return GL_UNSIGNED_BYTE;
		case IndexDataType::Unsigned_Short: return GL_UNSIGNED_SHORT;
		case IndexDataType::Unsigned_Int: return GL_UNSIGNED_INT;
		}

		DEER_CORE_ERROR("Unkown shader index data type");
	}
}
