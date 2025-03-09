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
#include "Deer/Voxels/VoxelData.h"

#include "Deer/Core/Log.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"

namespace Deer {
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

		VoxelData::getVoxelColorTextureAtlas()->bind(0);

		for (int x = 0; x < m_worldProps.getChunkCount(); x++) {
			ChunkRender& chunkRender = m_chunksRender[x];
			if (!chunkRender.hasData)
				continue;

			ChunkID chunkID = m_worldProps.getChunkID(x);
			chunkRender.solidVoxel->bind();

			VoxelData::getVoxelColorTextureAtlas()->bind(0);
			
			int assetID = AssetManager::loadAsset<Shader>(DataStore::rootPath / "assets/shaders/solid_voxel.glsl");
			Asset<Shader>& shaderAsset = AssetManager::getAsset<Shader>(assetID);

			shaderAsset.value->bind();
			shaderAsset.value->uploadUniformMat4("u_viewMatrix", cameraProjectionMatrix);
			shaderAsset.value->uploadUniformMat4("u_worldMatrix", glm::mat4(1.0f));
			shaderAsset.value->uploadUniformInt("u_texture", 0);
			shaderAsset.value->uploadUniformInt("u_textureSize", VoxelData::getVoxelTextureAtlasSize());
			shaderAsset.value->uploadUniformInt("u_chunkID_x", chunkID.x);
			shaderAsset.value->uploadUniformInt("u_chunkID_y", chunkID.y);
			shaderAsset.value->uploadUniformInt("u_chunkID_z", chunkID.z);
			
			Render::submit(chunkRender.solidVoxel);
		}
	}
}