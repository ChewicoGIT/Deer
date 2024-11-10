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
		std::vector<int> indices;
		std::vector<int> vertices;
	};
}

