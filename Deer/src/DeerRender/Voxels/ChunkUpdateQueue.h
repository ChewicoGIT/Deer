#pragma once
#include "Deer/Voxels/Voxel.h"
#include <queue>
#include <unordered_set>

namespace Deer {
	class ChunkUpdateQueue {
	public:
		// maybe add priority but for future...
		void addChunk(ChunkID);
		ChunkID pullChunk();
		bool hasChunk();
	private:
		std::queue<ChunkID> m_updateOrder;
		std::unordered_set<ChunkID, ChunkIDHash> m_containingChunks;
	};
}

