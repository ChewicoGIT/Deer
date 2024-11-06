#include "ChunkBaker.h"
#include "Deer/Render/Buffer.h"

namespace Deer {
	Ref<VertexArray> Deer::ChunkBaker::bakeChunk(Chunk& chunk) {
		vertices.clear();
		indices.clear();

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);

		vertices.push_back(0);
		vertices.push_back(0);
		vertices.push_back(0);

		vertices.push_back(0);
		vertices.push_back(1);
		vertices.push_back(0);

		vertices.push_back(1);
		vertices.push_back(0);
		vertices.push_back(0);

		Ref<VertexArray> va = VertexArray::create();
		Ref<VertexBuffer> vb = VertexBuffer::create(vertices.data(), vertices.size());
		Ref<IndexBuffer> ib = IndexBuffer::create(indices.data(), indices.size(), IndexDataType::Unsigned_Int);

		BufferLayout layout({ {"a_position", DataType::Int, ShaderDataType::FloatingPoint} });
		vb->setLayout(layout);

		va->addVertexBuffer(vb);
		va->setIndexBuffer(ib);

		return va;
	}
}
