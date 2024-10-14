#pragma once
#include "Deer/Render/VertexArray.h"
#include "Deer/Render/Buffer.h"
#include "Deer/Core/Core.h"

#include <vector>
#include <string>

namespace Deer {
	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		OpenGLVertexArray(const std::string& filePath);
		~OpenGLVertexArray() override;

		void bind() const override;
		void unbind() const override;

		void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		const Ref<IndexBuffer>& getIndexBuffer() override { return m_indexBuffer; }
	private:
		unsigned int index = 0;
		unsigned int m_vertexArray;
		Ref<IndexBuffer> m_indexBuffer;
		std::vector<Ref<VertexBuffer>> m_vertexBuffers;
	};
}

