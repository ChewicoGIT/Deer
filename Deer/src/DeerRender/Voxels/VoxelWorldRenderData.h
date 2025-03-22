#pragma once
#include "Deer/Memory.h"
#include "Deer/Voxel.h"

#include "DeerRender/Render/VertexArray.h"

#include <queue>
#include <vector>
#include <unordered_set>

namespace Deer {
	struct ChunkRender {
		Ref<VertexArray> solidVoxel;
		bool hasData = false;
	};

	struct SolidVoxelVertexData {
		uint16_t textureID = 0;
		uint8_t ambient_occlusion = 0;

		uint8_t xPos = 0;
		uint8_t yPos = 0;
		uint8_t zPos = 0;

		uint8_t a_light = 0;
		uint8_t r_light = 0;
		uint8_t g_light = 0;
		uint8_t b_light = 0;

		uint8_t normal = 0;

		uint8_t u = 0;
		uint8_t v = 0;

		SolidVoxelVertexData() = default;
	};

	class ChunkUpdateQueue {
	public:
		// TODO: Add priority
		void addChunk(ChunkID);
		ChunkID pullChunk();
		bool hasChunk();
	private:
		std::queue<ChunkID> m_updateOrder;
		std::unordered_set<ChunkID, ChunkIDHash> m_containingChunks;
	};

    struct VoxelWorldRenderData {
        VoxelWorldRenderData(size_t chunkCount) {
            chunksRender = MakeScope<ChunkRender[]>(chunkCount);
        }
        
        // Chunk render data
        Scope<ChunkRender[]> chunksRender;
        ChunkUpdateQueue chunkQueue;
    
        // Voxel creation data
        std::vector<uint32_t> indices;
        std::vector<SolidVoxelVertexData> vertexData;
        std::queue<VoxelCordinates> ambientLightPropagation;
        std::queue<VoxelCordinates> voxelLightPropagation;
        std::vector<VoxelCordinates> tmp_voxelLightSource;
    };
}