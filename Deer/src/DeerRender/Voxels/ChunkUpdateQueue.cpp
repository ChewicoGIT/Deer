#include "ChunkUpdateQueue.h"

namespace Deer {
	void ChunkUpdateQueue::addChunk(ChunkID chunkID) {
		if (m_containingChunks.contains(chunkID))
			return;

		m_containingChunks.insert(chunkID);
		m_updateOrder.push(chunkID);
	}

	ChunkID ChunkUpdateQueue::pullChunk() {
		ChunkID chunkID = m_updateOrder.front();
		m_updateOrder.pop();
		m_containingChunks.erase(chunkID);
		return chunkID;
	}

	bool ChunkUpdateQueue::hasChunk() {
		return m_updateOrder.size() > 0;
	}
}
