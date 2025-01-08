#include "ChunkBaker.h"
#include "DeerRender/Render/Buffer.h"

namespace Deer {

	void ChunkBaker::genSolidVoxel(ChunkVoxelID id) {
		// for each dir
		for (int i = 0; i < 6; i++) {
			Voxel voxel = m_workingChunk->readVoxel(id);
			bool isSolid = voxel.id != 0;

			if (!isSolid)
				continue;

			int nextX = NORMAL_DIR(INTERNAL_X_POS, i) + id.x;
			int nextY = NORMAL_DIR(INTERNAL_Y_POS, i) + id.y;
			int nextZ = NORMAL_DIR(INTERNAL_Z_POS, i) + id.z;

			bool nextIsSolid = true;
			if (nextX < 0 || nextY < 0 || nextZ < 0 || nextX >= CHUNK_SIZE_X || nextY >= CHUNK_SIZE_Y || nextZ >= CHUNK_SIZE_Z) { 
				nextIsSolid = false;
			}
			else {
				Voxel nextVoxel = m_workingChunk->readVoxel(ChunkVoxelID(nextX, nextY, nextZ));
				nextIsSolid = nextVoxel.id != 0;
			}

			if (nextIsSolid)
				continue;

			int vertexID = m_solidVoxelVertices.size();
			m_indices.push_back(vertexID);
			m_indices.push_back(vertexID + 2);
			m_indices.push_back(vertexID + 1);

			m_indices.push_back(vertexID + 1);
			m_indices.push_back(vertexID + 2);
			m_indices.push_back(vertexID + 3);

			for (int v = 0; v < 4; v++) {
				SolidVoxelVertexData vertexData(
					id.x + NORMAL_VERTEX_POS(INTERNAL_X_POS, v, i),
					id.y + NORMAL_VERTEX_POS(INTERNAL_Y_POS, v, i),
					id.z + NORMAL_VERTEX_POS(INTERNAL_Z_POS, v, i),
					i, VERTEX_UV(INTERNAL_X_POS, v), VERTEX_UV(INTERNAL_Y_POS, v));

				m_solidVoxelVertices.push_back(vertexData);
			}
		}
	}

	void ChunkBaker::bakeSolidVoxel() {
		
		for (int x = 0; x < CHUNK_SIZE_X; x++) {
			for (int y = 0; y < CHUNK_SIZE_Y; y++) {
				for (int z = 0; z < CHUNK_SIZE_Z; z++) {
					genSolidVoxel(ChunkVoxelID(x, y, z));
				}
			}
		}
	}

	ChunkRender Deer::ChunkBaker::bakeChunk(Chunk& chunk) {
		m_solidVoxelVertices.clear();
		m_indices.clear();
		m_workingChunk = &chunk;

		bakeSolidVoxel();

		Ref<VertexArray> va = VertexArray::create();
		va->bind();
		Ref<VertexBuffer> vb = VertexBuffer::create(m_solidVoxelVertices.data(), m_solidVoxelVertices.size() * sizeof(SolidVoxelVertexData));
		Ref<IndexBuffer> ib = IndexBuffer::create(m_indices.data(), m_indices.size() * sizeof(unsigned int), IndexDataType::Unsigned_Int);

		BufferLayout layout({ 
			{ "a_xPos", DataType::Unsigned_Byte, ShaderDataType::FloatingPoint },
			{ "a_yPos", DataType::Unsigned_Byte, ShaderDataType::FloatingPoint },
			{ "a_zPos", DataType::Unsigned_Byte, ShaderDataType::FloatingPoint },
			{ "a_normal", DataType::Unsigned_Byte, ShaderDataType::Integer },
			{ "a_u", DataType::Unsigned_Byte, ShaderDataType::FloatingPoint },
			{ "a_v", DataType::Unsigned_Byte, ShaderDataType::FloatingPoint }
			});
		vb->setLayout(layout);
		va->addVertexBuffer(vb);
		va->setIndexBuffer(ib);

		ChunkRender chunkRender;
		chunkRender.solidVoxel = va;
		chunkRender.hasData = true;

		return chunkRender;
	}

}
