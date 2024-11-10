#include "ChunkBaker.h"
#include "Deer/Render/Buffer.h"

namespace Deer {
	ChunkRender Deer::ChunkBaker::bakeChunk(Chunk& chunk) {
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
		va->bind();
		Ref<VertexBuffer> vb = VertexBuffer::create(vertices.data(), vertices.size() * sizeof(int));
		Ref<IndexBuffer> ib = IndexBuffer::create(indices.data(), indices.size() * sizeof(unsigned int), IndexDataType::Unsigned_Int);
		
		BufferLayout layout({ {"a_position", DataType::Int, ShaderDataType::FloatingPoint} });
		vb->setLayout(layout);

		va->addVertexBuffer(vb);
		va->setIndexBuffer(ib);
		
		ChunkRender chunkRender;
		chunkRender.solidChunk = va;

		return chunkRender;
	}
}
