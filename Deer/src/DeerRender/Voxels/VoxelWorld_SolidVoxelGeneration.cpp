#include "Deer/Voxels/VoxelWorld.h"
#include "Deer/Voxels/Chunk.h"
#include "Deer/Core/Application.h"
#include "Deer/Core/Project.h"
#include "Deer/Asset/AssetManager.h"
#include "Deer/Scene/Entity.h"
#include "Deer/Scene/Components.h"
#include "DeerRender/Render/Render.h"
#include "DeerRender/Render/RenderUtils.h"
#include "DeerRender/Render/Texture.h"
#include "DeerRender/Voxels/ChunkRender.h"

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
			{ "a_textureID", DataType::Unsigned_Short2, ShaderDataType::Integer, offsetof(SolidVoxelVertexData, textureID) },
			{ "a_xPos", DataType::Unsigned_Byte, ShaderDataType::FloatingPoint, offsetof(SolidVoxelVertexData, xPos) },
			{ "a_yPos", DataType::Unsigned_Byte, ShaderDataType::FloatingPoint, offsetof(SolidVoxelVertexData, yPos) },
			{ "a_zPos", DataType::Unsigned_Byte, ShaderDataType::FloatingPoint, offsetof(SolidVoxelVertexData, zPos) },
			{ "a_normal", DataType::Unsigned_Byte, ShaderDataType::Integer, offsetof(SolidVoxelVertexData, normal) },
			{ "a_u", DataType::Unsigned_Byte, ShaderDataType::FloatingPoint, offsetof(SolidVoxelVertexData, u) },
			{ "a_v", DataType::Unsigned_Byte, ShaderDataType::FloatingPoint, offsetof(SolidVoxelVertexData, v) },
			{ "a_ambient_light", DataType::Unsigned_Byte, ShaderDataType::FloatingPoint, offsetof(SolidVoxelVertexData, ambient_light)},
			{ "a_r_l", DataType::Unsigned_Byte, ShaderDataType::FloatingPoint, offsetof(SolidVoxelVertexData, voxel_red_light) },
			{ "a_g_l", DataType::Unsigned_Byte, ShaderDataType::FloatingPoint, offsetof(SolidVoxelVertexData, voxel_green_light) },
			{ "a_b_l", DataType::Unsigned_Byte, ShaderDataType::FloatingPoint, offsetof(SolidVoxelVertexData, voxel_blue_light) }
		}, sizeof(SolidVoxelVertexData));
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

		if (!voxel.isVoxelType())
			return;

		for (int i = 0; i < 6; i++) {
			// facing means the front voxel of the face
			int facingX = NORMAL_DIR(X_AXIS, i) + chunkVoxelID.x + CHUNK_SIZE_X * chunkID.x;
			int facingY = NORMAL_DIR(Y_AXIS, i) + chunkVoxelID.y + CHUNK_SIZE_Y * chunkID.y;
			int facingZ = NORMAL_DIR(Z_AXIS, i) + chunkVoxelID.z + CHUNK_SIZE_Z * chunkID.z;

			Voxel frontVoxel = readVoxel(facingX, facingY, facingZ);
			if (frontVoxel.isVoxelType())
				continue;

			VoxelLight frontVoxelLight = readLight(facingX, facingY, facingZ);

			// next is the voxel 2 m away from the normal
			int nextX = NORMAL_DIR(X_AXIS, i) * 2 + chunkVoxelID.x + CHUNK_SIZE_X * chunkID.x;
			int nextY = NORMAL_DIR(Y_AXIS, i) * 2 + chunkVoxelID.y + CHUNK_SIZE_Y * chunkID.y;
			int nextZ = NORMAL_DIR(Z_AXIS, i) * 2 + chunkVoxelID.z + CHUNK_SIZE_Z * chunkID.z;

			Voxel nextVoxel = readVoxel(nextX, nextY, nextZ);
			VoxelLight nextVoxelLight = readLight(nextX, nextY, nextZ);

			bool isFaceShadow = frontVoxelLight.ambient_light != 255 && (nextVoxel.isVoxelType() || frontVoxelLight.ambient_light > nextVoxelLight.ambient_light);

			int vertexID = m_vertexData.size();
			int ambient_oclusion[4] = { frontVoxelLight.ambient_light, frontVoxelLight.ambient_light, frontVoxelLight.ambient_light, frontVoxelLight.ambient_light };
			int voxel_light[4 * 3];
			int air_count[4];

			for (int v = 0; v < 4; v++) {
				voxel_light[v * 3 + 0] = frontVoxelLight.r_light;
				voxel_light[v * 3 + 1] = frontVoxelLight.g_light;
				voxel_light[v * 3 + 2] = frontVoxelLight.b_light;

				bool airEdge[3] = { false };
				bool faceShadow[3] = { false };
				// Calculate ambient occlusion
				for (int a = 0; a < 2; a++) {
					int checkDirX = facingX + AMBIENT_OCCLUSION_VERTEX(X_AXIS, a, v, i);
					int checkDirY = facingY + AMBIENT_OCCLUSION_VERTEX(Y_AXIS, a, v, i);
					int checkDirZ = facingZ + AMBIENT_OCCLUSION_VERTEX(Z_AXIS, a, v, i);

					Voxel voxelData = readVoxel(checkDirX, checkDirY, checkDirZ);
					VoxelLight lightData = readLight(checkDirX, checkDirY, checkDirZ);

					airEdge[a] = !voxelData.isVoxelType();
					ambient_oclusion[v] += lightData.ambient_light;
					voxel_light[v * 3 + 0] += lightData.r_light;
					voxel_light[v * 3 + 1] += lightData.g_light;
					voxel_light[v * 3 + 2] += lightData.b_light;


					if (airEdge[a]) {
						int checkNextDirX = checkDirX + NORMAL_DIR(X_AXIS, i);
						int checkNextDirY = checkDirY + NORMAL_DIR(Y_AXIS, i);
						int checkNextDirZ = checkDirZ + NORMAL_DIR(Z_AXIS, i);

						Voxel nextVoxelData = readVoxel(checkNextDirX, checkNextDirY, checkNextDirZ);
						VoxelLight nextLightData = readLight(checkNextDirX, checkNextDirY, checkNextDirZ);
						faceShadow[a] = lightData.ambient_light != 255 && (nextVoxelData.isVoxelType() || lightData.ambient_light > nextLightData.ambient_light);
					}
					else
						faceShadow[a] = true;
				}

				if (airEdge[0] || airEdge[1]) {
					int checkDirX = facingX + AMBIENT_OCCLUSION_VERTEX(X_AXIS, 0, v, i) + AMBIENT_OCCLUSION_VERTEX(X_AXIS, 1, v, i);
					int checkDirY = facingY + AMBIENT_OCCLUSION_VERTEX(Y_AXIS, 0, v, i) + AMBIENT_OCCLUSION_VERTEX(Y_AXIS, 1, v, i);
					int checkDirZ = facingZ + AMBIENT_OCCLUSION_VERTEX(Z_AXIS, 0, v, i) + AMBIENT_OCCLUSION_VERTEX(Z_AXIS, 1, v, i);

					Voxel voxelData = readVoxel(checkDirX, checkDirY, checkDirZ);
					VoxelLight lightData = readLight(checkDirX, checkDirY, checkDirZ);

					airEdge[2] = !voxelData.isVoxelType();
					ambient_oclusion[v] += lightData.ambient_light;
					voxel_light[v * 3 + 0] += lightData.r_light;
					voxel_light[v * 3 + 1] += lightData.g_light;
					voxel_light[v * 3 + 2] += lightData.b_light;

					if (airEdge[2]) {
						int checkNextDirX = checkDirX + NORMAL_DIR(X_AXIS, i);
						int checkNextDirY = checkDirY + NORMAL_DIR(Y_AXIS, i);
						int checkNextDirZ = checkDirZ + NORMAL_DIR(Z_AXIS, i);

						Voxel nextVoxelData = readVoxel(checkNextDirX, checkNextDirY, checkNextDirZ);
						VoxelLight nextLightData = readLight(checkNextDirX, checkNextDirY, checkNextDirZ);
						faceShadow[2] = lightData.ambient_light != 255 && (nextVoxelData.isVoxelType() || lightData.ambient_light > nextLightData.ambient_light);
					}
					else
						faceShadow[2] = true;
				}
				else
					airEdge[2] = true;

				air_count[v] = (int)airEdge[0] + (int)airEdge[1] + (int)airEdge[2] + 1;

				ambient_oclusion[v] = (ambient_oclusion[v] * 4) / (air_count[v] * 3 + 4);
				voxel_light[v * 3 + 0] = (voxel_light[v * 3 + 0] * 4) / (air_count[v] * 3 + 4);
				voxel_light[v * 3 + 1] = (voxel_light[v * 3 + 1] * 4) / (air_count[v] * 3 + 4);
				voxel_light[v * 3 + 2] = (voxel_light[v * 3 + 2] * 4) / (air_count[v] * 3 + 4);

				int faceShadowC = (int)isFaceShadow + (int)faceShadow[0] + (int)faceShadow[1] + (int)faceShadow[2];

				//ambient_oclusion[v] = ambient_oclusion[v] * (22 - faceShadowC) / 20;//
				ambient_oclusion[v] = (ambient_oclusion[v] > 255) ? 255 : ambient_oclusion[v];

				SolidVoxelVertexData vertexData(
					chunkVoxelID.x + NORMAL_VERTEX_POS(X_AXIS, v, i),
					chunkVoxelID.y + NORMAL_VERTEX_POS(Y_AXIS, v, i),
					chunkVoxelID.z + NORMAL_VERTEX_POS(Z_AXIS, v, i),
					i, VERTEX_UV(X_AXIS, v), VERTEX_UV(Y_AXIS, v),
					ambient_oclusion[v],
					voxel.getVoxelTextureID(i),
					voxel_light[v * 3 + 0],
					voxel_light[v * 3 + 1],
					voxel_light[v * 3 + 2]);

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