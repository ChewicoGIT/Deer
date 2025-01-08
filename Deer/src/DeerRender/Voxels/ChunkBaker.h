#pragma once
#include "Deer/Voxels/Chunk.h"
#include "DeerRender/Render/VertexArray.h"
#include "DeerRender/Voxels/ChunkRender.h"
#include <vector>

namespace Deer {
	class ChunkBaker {
	public:
		ChunkRender bakeChunk(Chunk& chunk);
	private:
		std::vector<int> m_indices;
		std::vector<SolidVoxelVertexData> m_solidVoxelVertices;
		Chunk* m_workingChunk;

		void bakeSolidVoxel();
		void genSolidVoxel(ChunkVoxelID id);
	};
}

