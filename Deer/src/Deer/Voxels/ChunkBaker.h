#pragma once
#include "Deer/Voxels/Chunk.h"
#include "Deer/Render/VertexArray.h"
#include "Deer/Voxels/ChunkRender.h"
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
		void genSolidVoxel(ChunkVoxelID voxelID);
	};
}

