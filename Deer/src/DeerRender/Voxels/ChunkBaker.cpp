#include "ChunkBaker.h"
#include "DeerRender/Render/Buffer.h"

namespace Deer {

	void ChunkBaker::genSolidVoxel(ChunkVoxelID voxelID) {
		// for each dir
		for (int i = 0; i < 6; i++) {
			Voxel& voxel = m_workingChunk->getVoxel(voxelID);
			bool isSolid = voxel.id != 0;

			if (!isSolid)
				continue;

			int nextX = NORMAL_DIR(0, i) + voxelID.x;
			int nextY = NORMAL_DIR(1, i) + voxelID.y;
			int nextZ = NORMAL_DIR(2, i) + voxelID.z;

			bool nextIsSolid = true;
			if (nextX < 0 || nextY < 0 || nextZ < 0 || nextX >= CHUNK_SIZE_X || nextY >= CHUNK_SIZE_Y || nextZ >= CHUNK_SIZE_Z) { 
				nextIsSolid = false;
			}
			else {
				Voxel& nextVoxel = m_workingChunk->getVoxel(ChunkVoxelID(nextX, nextY, nextZ));
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
					voxelID.x + NORMAL_VERTEX_POS_DENORMALIZED(0, v, i),
					voxelID.y + NORMAL_VERTEX_POS_DENORMALIZED(1, v, i),
					voxelID.z + NORMAL_VERTEX_POS_DENORMALIZED(2, v, i),
					i, VERTEX_UV(0, v), VERTEX_UV(1, v));

				int x = vertexData.getXPosition();
				int y = vertexData.getYPosition();
				int z = vertexData.getZPosition();

				int normal = vertexData.getNormal();

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

		BufferLayout layout({ {"a_vertexData", DataType::Unsigned_Int, ShaderDataType::Integer} });
		vb->setLayout(layout);
		va->addVertexBuffer(vb);
		va->setIndexBuffer(ib);

		ChunkRender chunkRender;
		chunkRender.solidVoxel = va;

		return chunkRender;
	}

}
