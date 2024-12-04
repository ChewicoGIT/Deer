#pragma once
#include "Deer/Voxels/Voxel.h"
#include <queue>

namespace Deer {
	class ChunkUpdateQueue {
	public:
		// maybe priority but for future...
		void addChunk(ChunkID);
		ChunkID pullChunk();
	private:
		std::queue<ChunkID> m_updateOrder;
	};
}

