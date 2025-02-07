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
	void VoxelWorld::bakeChunk(int x, int y, int z) {
		ChunkID chunkID(x, y, z);
		bakeChunk(chunkID);
	}

	void VoxelWorld::bakeNextChunk() {
		if (!m_chunkQueue.hasChunk())
			return;

		ChunkID nextChunk = m_chunkQueue.pullChunk();
		bakeChunk(nextChunk);
	}

	void VoxelWorld::bakeChunk(ChunkID chunkID) {
		int chunk_internal_id = m_worldProps.getInternalID(chunkID);

		Chunk& chunk = m_chunks[chunk_internal_id];
		m_chunksRender[chunk_internal_id] = m_chunkBaker.bakeChunk(chunk);
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

			ChunkID chunkID = m_worldProps.getChunkPosition(x);
			chunkRender.solidVoxel->bind();

			int textureAssetID = AssetManager::loadAsset<Texture2D>("assets/Textures/Dirt.png");
			Asset<Texture2D>& textureAsset = AssetManager::getAsset<Texture2D>(textureAssetID);
			textureAsset.value->bind(0);

			int assetID = AssetManager::loadAsset<Shader>("assets/Shaders/SolidVoxel.glsl");
			Asset<Shader>& shaderAsset = AssetManager::getAsset<Shader>(assetID);

			shaderAsset.value->bind();
			shaderAsset.value->uploadUniformMat4("u_viewMatrix", cameraProjectionMatrix);
			shaderAsset.value->uploadUniformMat4("u_worldMatrix", glm::mat4(1.0f));
			shaderAsset.value->uploadUniformInt("u_objectID", -1);
			shaderAsset.value->uploadUniformInt("u_texture", 0);
			shaderAsset.value->uploadUniformInt("u_chunkID_x", chunkID.x);
			shaderAsset.value->uploadUniformInt("u_chunkID_y", chunkID.y);
			shaderAsset.value->uploadUniformInt("u_chunkID_z", chunkID.z);

			Render::submit(chunkRender.solidVoxel);
		}
	}
}