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
			{ "a_ambient_occlusion", DataType::Unsigned_Byte, ShaderDataType::FloatingPoint, offsetof(SolidVoxelVertexData, ambient_occlusion) },

			{ "a_xPos", DataType::Unsigned_Byte, ShaderDataType::FloatingPoint, offsetof(SolidVoxelVertexData, xPos) },
			{ "a_yPos", DataType::Unsigned_Byte, ShaderDataType::FloatingPoint, offsetof(SolidVoxelVertexData, yPos) },
			{ "a_zPos", DataType::Unsigned_Byte, ShaderDataType::FloatingPoint, offsetof(SolidVoxelVertexData, zPos) },

			{ "a_a_light", DataType::Unsigned_Byte, ShaderDataType::FloatingPoint, offsetof(SolidVoxelVertexData, a_light) },
			{ "a_r_light", DataType::Unsigned_Byte, ShaderDataType::FloatingPoint, offsetof(SolidVoxelVertexData, r_light) },
			{ "a_g_light", DataType::Unsigned_Byte, ShaderDataType::FloatingPoint, offsetof(SolidVoxelVertexData, g_light) },
			{ "a_b_light", DataType::Unsigned_Byte, ShaderDataType::FloatingPoint, offsetof(SolidVoxelVertexData, b_light) },

			{ "a_normal", DataType::Unsigned_Byte, ShaderDataType::Integer, offsetof(SolidVoxelVertexData, normal)},

			{ "a_u", DataType::Unsigned_Byte, ShaderDataType::FloatingPoint, offsetof(SolidVoxelVertexData, u) },
			{ "a_v", DataType::Unsigned_Byte, ShaderDataType::FloatingPoint, offsetof(SolidVoxelVertexData, v) }
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
			// front means the front voxel of the face
			VoxelCordinates frontID(		
				NORMAL_DIR(X_AXIS, i) + chunkVoxelID.x + CHUNK_SIZE_X * chunkID.x,
				NORMAL_DIR(Y_AXIS, i) + chunkVoxelID.y + CHUNK_SIZE_Y * chunkID.y,
				NORMAL_DIR(Z_AXIS, i) + chunkVoxelID.z + CHUNK_SIZE_Z * chunkID.z
			);
			
			Voxel frontVoxel = readVoxel(frontID.x, frontID.y, frontID.z);
			// If the face is inside 2 Voxels we will not render it
			if (frontVoxel.isVoxelType())
				continue;

			VoxelLight frontVoxelLight = readLight(frontID.x, frontID.y, frontID.z);

			// front2ID means the front voxel in 2 voxels apart
			VoxelCordinates front2ID(
				NORMAL_DIR(X_AXIS, i) * 2 + chunkVoxelID.x + CHUNK_SIZE_X * chunkID.x,
				NORMAL_DIR(Y_AXIS, i) * 2 + chunkVoxelID.y + CHUNK_SIZE_Y * chunkID.y,
				NORMAL_DIR(Z_AXIS, i) * 2 + chunkVoxelID.z + CHUNK_SIZE_Z * chunkID.z
			);
			
			Voxel front2Voxel = readVoxel(front2ID.x, front2ID.y, front2ID.z);
			VoxelLight front2VoxelLight = readLight(front2ID.x, front2ID.y, front2ID.z);

			// Face Shadow means that face is not in the direction of the light
			bool isFaceShadow = frontVoxelLight.ambient_light != 255 && (frontVoxel.isVoxelType() || frontVoxelLight.ambient_light > front2VoxelLight.ambient_light);

			// Save the vertex id for later
			int vertexID = m_vertexData.size();

			int voxel_count[4];

			// For every vertex
			for (int v = 0; v < 4; v++) {
				// This var takes the count of the voxels that where added in voxel_light
				int sample_count = 1;
				// Count the blocks to calculate the ambient occlusion, min value 0 max value 2
				voxel_count[v] = 0;

				// col 0 -> ambient oclusion
				// col 1 -> red light
				// col 2 -> green light
				// col 3 -> blue light
				int voxel_light[4];
			
				voxel_light[0] = frontVoxelLight.ambient_light;
				voxel_light[1] = frontVoxelLight.r_light;
				voxel_light[2] = frontVoxelLight.g_light;
				voxel_light[3] = frontVoxelLight.b_light;

				//Checks if there is air on the blocks of the side 
				bool airEdge[2];

				// Calculate ambient occlusion and light difusion
				for (int a = 0; a < 2; a++) {
					VoxelCordinates checkChordsID(
						frontID.x + AMBIENT_OCCLUSION_VERTEX(X_AXIS, a, v, i),
						frontID.y + AMBIENT_OCCLUSION_VERTEX(Y_AXIS, a, v, i),
						frontID.z + AMBIENT_OCCLUSION_VERTEX(Z_AXIS, a, v, i)
					);

					Voxel checkChordsVoxel = readVoxel(checkChordsID.x, checkChordsID.y, checkChordsID.z);
					VoxelLight checkChordsVoxelLight = readLight(checkChordsID.x, checkChordsID.y, checkChordsID.z);

					// Check for the same chords but 2 voxels apart
					airEdge[a] = !checkChordsVoxel.isVoxelType();
					if (airEdge[a]) {
						sample_count++;

						voxel_light[0] += checkChordsVoxelLight.ambient_light;
						voxel_light[1] += checkChordsVoxelLight.r_light;
						voxel_light[2] += checkChordsVoxelLight.g_light;
						voxel_light[3] += checkChordsVoxelLight.b_light;
					} else {
						voxel_count[v]++;
					}
				}

				if (airEdge[0] || airEdge[1]) {
					VoxelCordinates checkChordsID(
						frontID.x + AMBIENT_OCCLUSION_VERTEX(X_AXIS, 0, v, i) + AMBIENT_OCCLUSION_VERTEX(X_AXIS, 1, v, i),
						frontID.y + AMBIENT_OCCLUSION_VERTEX(Y_AXIS, 0, v, i) + AMBIENT_OCCLUSION_VERTEX(Y_AXIS, 1, v, i),
						frontID.z + AMBIENT_OCCLUSION_VERTEX(Z_AXIS, 0, v, i) + AMBIENT_OCCLUSION_VERTEX(Z_AXIS, 1, v, i)
					);

					Voxel checkChordsVoxel = readVoxel(checkChordsID.x, checkChordsID.y, checkChordsID.z);
					VoxelLight checkChordsVoxelLight = readLight(checkChordsID.x, checkChordsID.y, checkChordsID.z);

					if (!checkChordsVoxel.isVoxelType()) {
						sample_count++;

						voxel_light[0] += checkChordsVoxelLight.ambient_light;
						voxel_light[1] += checkChordsVoxelLight.r_light;
						voxel_light[2] += checkChordsVoxelLight.g_light;
						voxel_light[3] += checkChordsVoxelLight.b_light;
					} else {
						voxel_count[v]++;
					}
				}

				for (int xi = 0; xi < 4; xi++) {
					voxel_light[xi] = (voxel_light[xi]) / sample_count;
					voxel_light[xi] = (voxel_light[xi] > 255)? 255 : voxel_light[xi];
				}

				SolidVoxelVertexData vertex_data;
				vertex_data.textureID = voxel.getVoxelTextureID(i);
				vertex_data.ambient_occlusion = voxel_count[v];
				
				vertex_data.xPos = chunkVoxelID.x + NORMAL_VERTEX_POS(X_AXIS, v, i);
				vertex_data.yPos = chunkVoxelID.y + NORMAL_VERTEX_POS(Y_AXIS, v, i);
				vertex_data.zPos = chunkVoxelID.z + NORMAL_VERTEX_POS(Z_AXIS, v, i);
				
				vertex_data.a_light = voxel_light[0];
				vertex_data.r_light = voxel_light[1];
				vertex_data.g_light = voxel_light[2];
				vertex_data.b_light = voxel_light[3];

				vertex_data.normal = i;

				vertex_data.u = VERTEX_UV(X_AXIS, v);
				vertex_data.v = VERTEX_UV(Y_AXIS, v);

				m_vertexData.push_back(vertex_data);
			}

			if (voxel_count[0] + voxel_count[3] > voxel_count[1] + voxel_count[2]) {
				m_indices.push_back(vertexID);
				m_indices.push_back(vertexID + 2);
				m_indices.push_back(vertexID + 1);

				m_indices.push_back(vertexID + 1);
				m_indices.push_back(vertexID + 2);
				m_indices.push_back(vertexID + 3);
			} else {
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