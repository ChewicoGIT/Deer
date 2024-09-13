#pragma once
#include "Deer/Render/Buffer.h"

#include <memory>

namespace Deer {
	class VertexArray;
	using Mesh = VertexArray;

	class VertexArray {
	public:
		virtual ~VertexArray() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

		virtual const Ref<IndexBuffer>& getIndexBuffer() = 0;

		static Ref<VertexArray> create();
	};
}

