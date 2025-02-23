#include "Deer/Voxels/VoxelWorld.h"
#include "Deer/Core/Application.h"
#include "Deer/Core/Project.h"
#include "Deer/Asset/AssetManager.h"
#include "Deer/Scene/Entity.h"
#include "Deer/Scene/Components.h"
#include "DeerRender/Render/Render.h"
#include "DeerRender/Render/RenderUtils.h"
#include "DeerRender/Render/Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"

namespace Deer {
	void VoxelWorld::bakeNextChunk() {
		if (!m_chunkQueue.hasChunk())
			return;

		// Pull the next chunk to render
		ChunkID nextChunk = m_chunkQueue.pullChunk();
		m_vertexData.clear();
		m_indices.clear();

		// For each voxel
		for (int x = 0; x < CHUNK_SIZE_X; x++) {
			for (int y = 0; y < CHUNK_SIZE_Y; y++) {
				for (int z = 0; z < CHUNK_SIZE_Z; z++) {
					genSolidVoxel(nextChunk, ChunkVoxelID(x, y, z));
				}
			}
		}

		// Pass the data to the GPU
		Ref<VertexArray> va = VertexArray::create();
		va->bind();
		Ref<VertexBuffer> vb = VertexBuffer::create(m_vertexData.data(), m_vertexData.size() * sizeof(SolidVoxelVertexData));
		Ref<IndexBuffer> ib = IndexBuffer::create(m_indices.data(), m_indices.size() * sizeof(uint32_t), IndexDataType::Unsigned_Int);

		BufferLayout layout({
			{ "a_xPos", DataType::Unsigned_Byte, ShaderDataType::FloatingPoint },
			{ "a_yPos", DataType::Unsigned_Byte, ShaderDataType::FloatingPoint },
			{ "a_zPos", DataType::Unsigned_Byte, ShaderDataType::FloatingPoint },
			{ "a_normal", DataType::Unsigned_Byte, ShaderDataType::Integer },
			{ "a_u", DataType::Unsigned_Byte, ShaderDataType::FloatingPoint },
			{ "a_v", DataType::Unsigned_Byte, ShaderDataType::FloatingPoint },
			{ "ambient_light", DataType::Unsigned_Byte, ShaderDataType::FloatingPoint }
			});
		vb->setLayout(layout);
		va->addVertexBuffer(vb);
		va->setIndexBuffer(ib);

		// Update the data to the chunk render
		int id = m_worldProps.getWorldChunkID(nextChunk);
		ChunkRender& chunkRender = m_chunksRender[id];
		chunkRender.solidVoxel = va;
		chunkRender.hasData = true;
	}

	void VoxelWorld::genSolidVoxel(ChunkID chunkID, ChunkVoxelID chunkVoxelID) {
		Chunk& workingChunk = m_chunks[m_worldProps.getWorldChunkID(chunkID)];
		Voxel voxel = workingChunk.readVoxel(chunkVoxelID);

		// If voxel is air do not process
		if (voxel.id == 0)
			return;

		for (int i = 0; i < 6; i++) {
			// Next means the voxel in the face of the direction
			int nextX = NORMAL_DIR(X_AXIS, i) + chunkVoxelID.x;
			int nextY = NORMAL_DIR(Y_AXIS, i) + chunkVoxelID.y;
			int nextZ = NORMAL_DIR(Z_AXIS, i) + chunkVoxelID.z;

			Voxel nextVoxel;
			VoxelLight nextVoxelLight;

			int nextWorldX = nextX + CHUNK_SIZE_X * chunkID.x;
			int nextWorldY = nextY + CHUNK_SIZE_Y * chunkID.y;
			int nextWorldZ = nextZ + CHUNK_SIZE_Z * chunkID.z;

			nextVoxel = readVoxel(nextWorldX, nextWorldY, nextWorldZ);
			nextVoxelLight = readLight(nextWorldX, nextWorldY, nextWorldZ);

			if (nextVoxel.id != 0)
				continue;

			int vertexID = m_vertexData.size();
			int ambient_oclusion[4] = { nextVoxelLight.ambient_light, nextVoxelLight.ambient_light, nextVoxelLight.ambient_light, nextVoxelLight.ambient_light };
			int air_count[4];
			for (int v = 0; v < 4; v++) {

				bool airEdge[3] = { false };
				// Calculate ambient occlusion
				for (int a = 0; a < 2; a++) {
					int checkDirX = nextX + AMBIENT_OCCLUSION_VERTEX(X_AXIS, a, v, i);
					int checkDirY = nextY + AMBIENT_OCCLUSION_VERTEX(Y_AXIS, a, v, i);
					int checkDirZ = nextZ + AMBIENT_OCCLUSION_VERTEX(Z_AXIS, a, v, i);

					Voxel voxelData;
					VoxelLight lightData;

					int nextWorldX = checkDirX + CHUNK_SIZE_X * chunkID.x;
					int nextWorldY = checkDirY + CHUNK_SIZE_Y * chunkID.y;
					int nextWorldZ = checkDirZ + CHUNK_SIZE_Z * chunkID.z;

					voxelData = readVoxel(nextWorldX, nextWorldY, nextWorldZ);
					lightData = readLight(nextWorldX, nextWorldY, nextWorldZ);

					airEdge[a] = voxelData.id == 0;
					ambient_oclusion[v] += lightData.ambient_light;
				}

				if (airEdge[0] || airEdge[1]) {
					int checkDirX = nextX + AMBIENT_OCCLUSION_VERTEX(X_AXIS, 0, v, i) + AMBIENT_OCCLUSION_VERTEX(X_AXIS, 1, v, i);
					int checkDirY = nextY + AMBIENT_OCCLUSION_VERTEX(Y_AXIS, 0, v, i) + AMBIENT_OCCLUSION_VERTEX(Y_AXIS, 1, v, i);
					int checkDirZ = nextZ + AMBIENT_OCCLUSION_VERTEX(Z_AXIS, 0, v, i) + AMBIENT_OCCLUSION_VERTEX(Z_AXIS, 1, v, i);

					Voxel voxelData;
					VoxelLight lightData;

					int nextWorldX = checkDirX + CHUNK_SIZE_X * chunkID.x;
					int nextWorldY = checkDirY + CHUNK_SIZE_Y * chunkID.y;
					int nextWorldZ = checkDirZ + CHUNK_SIZE_Z * chunkID.z;

					voxelData = readVoxel(nextWorldX, nextWorldY, nextWorldZ);
					lightData = readLight(nextWorldX, nextWorldY, nextWorldZ);

					airEdge[2] = voxelData.id == 0;
					ambient_oclusion[v] += lightData.ambient_light;
				}
				else
					airEdge[2] = false;

				air_count[v] = (int)airEdge[0] + (int)airEdge[1] + (int)airEdge[2] + 1;
				ambient_oclusion[v] = (ambient_oclusion[v] * 4) / (air_count[v] * 3 + 4);
				//ambient_oclusion[v] += 50 * airEdgeCount;
				//ambient_oclusion[v] += (nextVoxelLight.ambient_light * airEdgeCount * 2) / 3;

				SolidVoxelVertexData vertexData(
					chunkVoxelID.x + NORMAL_VERTEX_POS(X_AXIS, v, i),
					chunkVoxelID.y + NORMAL_VERTEX_POS(Y_AXIS, v, i),
					chunkVoxelID.z + NORMAL_VERTEX_POS(Z_AXIS, v, i),
					i, VERTEX_UV(X_AXIS, v), VERTEX_UV(Y_AXIS, v),
					ambient_oclusion[v]);

				m_vertexData.push_back(vertexData);
			}

			if ((ambient_oclusion[0] + ambient_oclusion[3]) < (ambient_oclusion[1] + ambient_oclusion[2])) {
				m_indices.push_back(vertexID);
				m_indices.push_back(vertexID + 2);
				m_indices.push_back(vertexID + 1);

				m_indices.push_back(vertexID + 1);
				m_indices.push_back(vertexID + 2);
				m_indices.push_back(vertexID + 3);
			}
			else {
				m_indices.push_back(vertexID);
				m_indices.push_back(vertexID + 3);
				m_indices.push_back(vertexID + 1);

				m_indices.push_back(vertexID + 2);
				m_indices.push_back(vertexID + 3);
				m_indices.push_back(vertexID);
			}

		}
	}

}