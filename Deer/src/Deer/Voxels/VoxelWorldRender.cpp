#include "Deer/Voxels/VoxelWorld.h"
#ifndef DEER_SERVICE
#include "Deer/Core/Application.h"
#include "Deer/Core/Project.h"
#include "Deer/Asset/AssetManager.h"
#include "Deer/Scene/Entity.h"
#include "Deer/Scene/Components.h"
#include "Deer/Render/Render.h"
#include "Deer/Render/RenderUtils.h"
#include "Deer/Render/Texture.h"

#include "Deer/Core/Log.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"

namespace Deer {
	void VoxelWorld::bakeChunk(int x, int y, int z) {
		ChunkID chunkID;
		chunkID.x = x + (1 << 10);
		chunkID.y = y + (1 << 10);
		chunkID.z = z + (1 << 10);

		bakeChunk(chunkID);
	}

	void VoxelWorld::bakeChunk(ChunkID chunkID) {
		// Add safe chunk loading
		Chunk& chunk = m_chunks[chunkID];
		m_chunksRender[chunkID] = m_chunkBaker.bakeChunk(chunk);
	}

	void VoxelWorld::render(SceneCamera camera) {
		glm::mat4 camMatrix = glm::inverse(camera.transform.getMatrix());
		glm::mat4 projectionMatrix = camera.camera.getMatrix();
		glm::mat4 invertZ = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, -1));

		// Lets invert the z axis for engine convenience
		glm::mat4 cameraProjectionMatrix = projectionMatrix * invertZ * camMatrix;
		for (auto& chunk : m_chunksRender) {
			int textureAssetID = Project::m_assetManager->loadAsset<Texture2D>("assets/Textures/Dirt.png");
			Asset<Texture2D>& textureAsset = Project::m_assetManager->getAsset<Texture2D>(textureAssetID);
			textureAsset.value->bind(0);

			int assetID = Project::m_assetManager->loadAsset<Shader>("assets/Shaders/SolidVoxel.glsl");
			Asset<Shader>& shaderAsset = Project::m_assetManager->getAsset<Shader>(assetID);

			shaderAsset.value->bind();
			shaderAsset.value->uploadUniformMat4("u_viewMatrix", cameraProjectionMatrix);
			shaderAsset.value->uploadUniformMat4("u_worldMatrix", glm::mat4(1.0f));
			shaderAsset.value->uploadUniformInt("u_objectID", -1);
			shaderAsset.value->uploadUniformInt("u_texture", 0);

			ChunkRender& chunkRender = chunk.second;
			chunkRender.solidVoxel->bind();

			Render::submit(chunkRender.solidVoxel);
		}
	}
}
#endif