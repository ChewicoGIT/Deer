#include "Deer/Voxels/VoxelWorld.h"
#include "Deer/Core/Application.h"
#include "Deer/Core/Project.h"
#include "Deer/Asset/AssetManager.h"
#include "Deer/Scene/Entity.h"
#include "Deer/Scene/Components.h"
#include "DeerRender/Render/Render.h"
#include "DeerRender/Render/RenderUtils.h"
#include "DeerRender/Render/Texture.h"

#include "Deer/Core/Log.h"
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

			// If facing voxel is out of chunk bound ask voxel world not the chunk
			if (nextX < 0 || nextY < 0 || nextZ < 0 || nextX >= CHUNK_SIZE_X || nextY >= CHUNK_SIZE_Y || nextZ >= CHUNK_SIZE_Z) {
				int nextWorldX = nextX + CHUNK_SIZE_X * chunkID.x;
				int nextWorldY = nextY + CHUNK_SIZE_Y * chunkID.y;
				int nextWorldZ = nextZ + CHUNK_SIZE_Z * chunkID.z;

				nextVoxel = readVoxel(nextWorldX, nextWorldY, nextWorldZ);
				nextVoxelLight = readLight(nextWorldX, nextWorldY, nextWorldZ);
			}
			else {
				ChunkVoxelID nextVoxelID(nextX, nextY, nextZ);

				nextVoxel = workingChunk.readVoxel(nextVoxelID);
				nextVoxelLight = workingChunk.readLight(nextVoxelID);
			}

			if (nextVoxel.id != 0)
				continue;

			int vertexID = m_vertexData.size();
			int ambient_oclusion[4] = { nextVoxelLight.ambient_light, nextVoxelLight.ambient_light, nextVoxelLight.ambient_light, nextVoxelLight.ambient_light };
			for (int v = 0; v < 4; v++) {

				bool airEdge[3] = { false };
				// Calculate ambient occlusion
				for (int a = 0; a < 2; a++) {
					int checkDirX = nextX + AMBIENT_OCCLUSION_VERTEX(X_AXIS, a, v, i);
					int checkDirY = nextY + AMBIENT_OCCLUSION_VERTEX(Y_AXIS, a, v, i);
					int checkDirZ = nextZ + AMBIENT_OCCLUSION_VERTEX(Z_AXIS, a, v, i);

					Voxel voxelData;
					VoxelLight lightData;
					if (checkDirX < 0 || checkDirY < 0 || checkDirZ < 0 || checkDirX >= CHUNK_SIZE_X || checkDirY >= CHUNK_SIZE_Y || checkDirZ >= CHUNK_SIZE_Z) {
						int nextWorldX = checkDirX + CHUNK_SIZE_X * chunkID.x;
						int nextWorldY = checkDirY + CHUNK_SIZE_Y * chunkID.y;
						int nextWorldZ = checkDirZ + CHUNK_SIZE_Z * chunkID.z;

						voxelData = readVoxel(nextWorldX, nextWorldY, nextWorldZ);
						lightData = readLight(nextWorldX, nextWorldY, nextWorldZ);
					}
					else {
						ChunkVoxelID nextVoxelID(checkDirX, checkDirY, checkDirZ);

						voxelData = workingChunk.readVoxel(nextVoxelID);
						lightData = workingChunk.readLight(nextVoxelID);
					}

					airEdge[a] = voxelData.id == 0;
					ambient_oclusion[v] += lightData.ambient_light;
				}

				if (airEdge[0] || airEdge[1]) {
					int checkDirX = nextX + AMBIENT_OCCLUSION_VERTEX(X_AXIS, 0, v, i) + AMBIENT_OCCLUSION_VERTEX(X_AXIS, 1, v, i);
					int checkDirY = nextY + AMBIENT_OCCLUSION_VERTEX(Y_AXIS, 0, v, i) + AMBIENT_OCCLUSION_VERTEX(Y_AXIS, 1, v, i);
					int checkDirZ = nextZ + AMBIENT_OCCLUSION_VERTEX(Z_AXIS, 0, v, i) + AMBIENT_OCCLUSION_VERTEX(Z_AXIS, 1, v, i);

					Voxel voxelData;
					VoxelLight lightData;
					if (checkDirX < 0 || checkDirY < 0 || checkDirZ < 0 || checkDirX >= CHUNK_SIZE_X || checkDirY >= CHUNK_SIZE_Y || checkDirZ >= CHUNK_SIZE_Z) {
						int nextWorldX = checkDirX + CHUNK_SIZE_X * chunkID.x;
						int nextWorldY = checkDirY + CHUNK_SIZE_Y * chunkID.y;
						int nextWorldZ = checkDirZ + CHUNK_SIZE_Z * chunkID.z;

						voxelData = readVoxel(nextWorldX, nextWorldY, nextWorldZ);
						lightData = readLight(nextWorldX, nextWorldY, nextWorldZ);
					}
					else {
						ChunkVoxelID nextVoxelID(checkDirX, checkDirY, checkDirZ);

						voxelData = workingChunk.readVoxel(nextVoxelID);
						lightData = workingChunk.readLight(nextVoxelID);
					}

					airEdge[2] = voxelData.id == 0;
					ambient_oclusion[v] += lightData.ambient_light;
				}
				else
					airEdge[2] = false;

				int airEdgeCount = (int)!airEdge[0] + (int)!airEdge[1];// +(int)!airEdge[2];
				ambient_oclusion[v] += 50 * airEdgeCount;
				//ambient_oclusion[v] += (nextVoxelLight.ambient_light * airEdgeCount * 2) / 3;

				SolidVoxelVertexData vertexData(
					chunkVoxelID.x + NORMAL_VERTEX_POS(X_AXIS, v, i),
					chunkVoxelID.y + NORMAL_VERTEX_POS(Y_AXIS, v, i),
					chunkVoxelID.z + NORMAL_VERTEX_POS(Z_AXIS, v, i),
					i, VERTEX_UV(X_AXIS, v), VERTEX_UV(Y_AXIS, v),
					ambient_oclusion[v] / 4);

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

	void VoxelWorld::resolveNextAmbientLightPropagation() {
		VoxelCordinates position = m_ambientLightPropagation.front();
		m_ambientLightPropagation.pop();

		VoxelLight currentLight = readLight(position.x, position.y, position.z);
		bool solidCheck[6] = { false };
		// Check for every simple dir
		for (int i = 0; i < 6; i++) {
			int nextX = position.x + NORMAL_DIR(X_AXIS, i);
			int nextY = position.y + NORMAL_DIR(Y_AXIS, i);
			int nextZ = position.z + NORMAL_DIR(Z_AXIS, i);

			Voxel nextVoxel = readVoxel(nextX, nextY, nextZ);
			solidCheck[i] = nextVoxel.id != 0;
			if (nextVoxel.id != 0)
				continue;

			VoxelLight nextLight = readLight(nextX, nextY, nextZ);
			uint8_t nextLightMinValue = currentLight.ambient_light - LIGHT_PROPAGATION_SIMPLE_FALL;

			if (nextLight.ambient_light < nextLightMinValue) {
				modLight(nextX, nextY, nextZ).ambient_light = nextLightMinValue;
				m_ambientLightPropagation.push(VoxelCordinates(nextX, nextY, nextZ));
			}
		}

		// Check for every complex dir
		for (int i = 0; i < 6; i++) {
			int cDir0 = LIGHT_PROPAGATION_COMPLEX_DIR(0, i);
			int cDir1 = LIGHT_PROPAGATION_COMPLEX_DIR(1, i);

			if (solidCheck[cDir0] || solidCheck[cDir1])
				continue;

			int nextX = position.x + NORMAL_DIR(X_AXIS, cDir0) + NORMAL_DIR(X_AXIS, cDir1);
			int nextY = position.y + NORMAL_DIR(Y_AXIS, cDir0) + NORMAL_DIR(Y_AXIS, cDir1);
			int nextZ = position.z + NORMAL_DIR(Z_AXIS, cDir0) + NORMAL_DIR(Z_AXIS, cDir1);

			Voxel nextVoxel = readVoxel(nextX, nextY, nextZ);
			solidCheck[i] = nextVoxel.id != 0;
			if (nextVoxel.id != 0)
				continue;

			VoxelLight nextLight = readLight(nextX, nextY, nextZ);
			uint8_t nextLightMinValue = currentLight.ambient_light - LIGHT_PROPAGATION_COMPLEX_FALL;

			if (nextLight.ambient_light < nextLightMinValue) {
				modLight(nextX, nextY, nextZ).ambient_light = nextLightMinValue;
				m_ambientLightPropagation.push(VoxelCordinates(nextX, nextY, nextZ));
			}
		}
	}

	void VoxelWorld::bakeAmbientLight(int minX, int maxX, int minZ, int maxZ) {
		int xPos = minX;
		for (; xPos <= maxX; xPos++) {
			int zPos = minZ;
			for (; zPos <= maxZ; zPos++) {
				LayerVoxel layer = readLayerVoxel(xPos, zPos);
				uint16_t maxHeight = layer.height;

				ChunkID chunkID;
				ChunkVoxelID chunkVoxelID;

				int yPos = 0;
				// All light blocks under the max height must be put to 0 ambient light and above to 255
				for (; yPos < maxHeight; yPos++) {
					extractChunkCordinates(xPos, yPos, zPos, chunkID, chunkVoxelID);
					Chunk& chunk = m_chunks[m_worldProps.getWorldChunkID(chunkID)];
					VoxelLight& voxelLight = chunk.modLight(chunkVoxelID);

					voxelLight.ambient_light = 0;
				}

				for (; yPos < m_worldProps.chunkSizeY * 32; yPos++) {
					extractChunkCordinates(xPos, yPos, zPos, chunkID, chunkVoxelID);
					Chunk& chunk = m_chunks[m_worldProps.getWorldChunkID(chunkID)];
					VoxelLight& voxelLight = chunk.modLight(chunkVoxelID);

					voxelLight.ambient_light = 255;
					m_ambientLightPropagation.push(VoxelCordinates(xPos, yPos, zPos));
				}
			}
		}

		//Resolve ambient light propagation
		while (!m_ambientLightPropagation.empty()) {
			resolveNextAmbientLightPropagation();
		}
	}

	VoxelLight VoxelWorld::readLight(int x, int y, int z) {
		ChunkID chunkID;
		ChunkVoxelID chunkVoxelID;

		extractChunkCordinates(x, y, z, chunkID, chunkVoxelID);
		if (!m_worldProps.isValid(chunkID))
			return lightVoxel;

		Chunk& chunk = m_chunks[m_worldProps.getWorldChunkID(chunkID)];
		return chunk.readLight(chunkVoxelID);
	}

	VoxelLight& VoxelWorld::modLight(int x, int y, int z) {
		ChunkID chunkID;
		ChunkVoxelID chunkVoxelID;

		extractChunkCordinates(x, y, z, chunkID, chunkVoxelID);
		if (!m_worldProps.isValid(chunkID))
			return lightVoxel;

		m_chunkQueue.addChunk(chunkID);

		Chunk& chunk = m_chunks[m_worldProps.getWorldChunkID(chunkID)];
		return chunk.modLight(chunkVoxelID);
	}

	void VoxelWorld::render(SceneCamera camera) {
		glm::mat4 camMatrix = glm::inverse(camera.transform.getMatrix());
		glm::mat4 projectionMatrix = camera.camera.getMatrix();
		glm::mat4 invertZ = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, -1));

		// Lets invert the z axis for engine convenience
		glm::mat4 cameraProjectionMatrix = projectionMatrix * invertZ * camMatrix;

		for (int x = 0; x < m_worldProps.getChunkCount(); x++) {
			ChunkRender& chunkRender = m_chunksRender[x];
			if (!chunkRender.hasData)
				continue;

			ChunkID chunkID = m_worldProps.getChunkID(x);
			chunkRender.solidVoxel->bind();

			int textureAssetID = AssetManager::loadAsset<Texture2D>("assets/Textures/Dirt.png");
			Asset<Texture2D>& textureAsset = AssetManager::getAsset<Texture2D>(textureAssetID);
			textureAsset.value->bind(0);

			int assetID = AssetManager::loadAsset<Shader>("assets/Shaders/SolidVoxel.glsl");
			Asset<Shader>& shaderAsset = AssetManager::getAsset<Shader>(assetID);

			shaderAsset.value->bind();
			shaderAsset.value->uploadUniformMat4("u_viewMatrix", cameraProjectionMatrix);
			shaderAsset.value->uploadUniformMat4("u_worldMatrix", glm::mat4(1.0f));
			shaderAsset.value->uploadUniformInt("u_texture", 0);
			shaderAsset.value->uploadUniformInt("u_chunkID_x", chunkID.x);
			shaderAsset.value->uploadUniformInt("u_chunkID_y", chunkID.y);
			shaderAsset.value->uploadUniformInt("u_chunkID_z", chunkID.z);
			
			Render::submit(chunkRender.solidVoxel);
		}
	}
}