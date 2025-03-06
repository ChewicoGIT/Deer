#pragma once
#include "DeerRender/Render/Buffer.h"

namespace Deer {

	unsigned int getOpenGLDataType(DataType type);
	unsigned int getOpenGLIndexDataType(IndexDataType type);

	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(void* data, unsigned int size);
		~OpenGLVertexBuffer() override;

		void bind() const override;
		void setLayout(BufferLayout& layout) override;
		BufferLayout& getLayout() override { return m_bufferLayout; }
	private:
		unsigned int m_bufferID;
		BufferLayout m_bufferLayout;
	};

	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(void* data, unsigned int size, IndexDataType indexDataType);
		~OpenGLIndexBuffer() override;

		void bind() const override;
		IndexDataType getIndexDataType() const override { return m_indexDataType; }
		unsigned int getCount() const override { return m_count; } 
	private:
		unsigned int m_bufferID;
		unsigned int m_count;
		IndexDataType m_indexDataType;
	};
}

