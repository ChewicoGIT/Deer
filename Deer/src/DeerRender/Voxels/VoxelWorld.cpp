#include "Deer/VoxelWorld.h"
#include "Deer/Voxels/Chunk.h"
#include "Deer/Application.h"
#include "Deer/Memory.h"
#include "Deer/Asset/AssetManager.h"
#include "Deer/Scene/Entity.h"
#include "Deer/Scene/Components.h"
#include "DeerRender/Render/Render.h"
#include "DeerRender/Render/RenderUtils.h"
#include "DeerRender/Render/Texture.h"
#include "DeerRender/Scene/SceneCamera.h"
#include "Deer/VoxelData.h"

#include "Deer/Core/Log.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"

#include "DeerRender/Voxels/VoxelWorldRenderData.h"

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

		m_renderData->chunkQueue.addChunk(chunkID);

		Chunk& chunk = m_chunks[m_worldProps.getWorldChunkID(chunkID)];
		return chunk.modLight(chunkVoxelID);
	}

	void VoxelWorld::render(const SceneCamera& camera) {
		glm::mat4 camMatrix = glm::inverse(camera.transform.getMatrix());
		glm::mat4 projectionMatrix = camera.camera.getMatrix();
		glm::mat4 invertZ = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, -1));

		// Lets invert the z axis for engine convenience
		glm::mat4 cameraProjectionMatrix = projectionMatrix * invertZ * camMatrix;

		VoxelData::getVoxelColorTextureAtlas()->bind(0);

		Ref<Shader>& shader = VoxelData::getSolidVoxelShader();
		shader->bind();

		shader->uploadUniformMat4("u_viewMatrix", cameraProjectionMatrix);
		shader->uploadUniformMat4("u_worldMatrix", glm::mat4(1.0f));
		shader->uploadUniformInt("u_texture", 0);
		shader->uploadUniformInt("u_textureSize", VoxelData::getVoxelTextureAtlasSize());

		for (int x = 0; x < m_worldProps.getChunkCount(); x++) {
			ChunkRender& chunkRender = m_renderData->chunksRender[x];
			if (!chunkRender.hasData)
				continue;

			ChunkID chunkID = m_worldProps.getChunkID(x);
			chunkRender.solidVoxel->bind();

			shader->uploadUniformInt("u_chunkID_x", chunkID.x);
			shader->uploadUniformInt("u_chunkID_y", chunkID.y);
			shader->uploadUniformInt("u_chunkID_z", chunkID.z);
			
			Render::submit(chunkRender.solidVoxel);
		}
	}
}