#pragma once
#include "Deer/Core/Core.h"
#include "Deer/Voxels/Voxel.h"

namespace Deer {
	struct VoxelWorldProps {
		uint8_t chunkSizeX;
		uint8_t chunkSizeY;
		uint8_t chunkSizeZ;

		VoxelWorldProps(uint8_t _chunkSizeX = 4, 
			uint8_t _chunkSizeY = 4, 
			uint8_t _chunkSizeZ = 2)
			: chunkSizeX(_chunkSizeX), 
			chunkSizeY(_chunkSizeY), 
			chunkSizeZ(_chunkSizeZ) { }

		inline int getChunkCount() {
			return chunkSizeX * chunkSizeY * chunkSizeZ;
		}

		inline int getInternalID(ChunkID chunkID) {
			return chunkID.z + chunkID.y * chunkSizeZ + chunkID.x * chunkSizeZ * chunkSizeY;
		}

		inline ChunkID getChunkPosition(int id) {
			ChunkID c_id;
			
			c_id.x = id / (chunkSizeZ * chunkSizeY);
			id -= c_id.x * (chunkSizeZ * chunkSizeY);

			c_id.y = id / chunkSizeZ;
			id -= c_id.y * chunkSizeZ;

			c_id.z = id;
			return c_id;
		}

		inline bool isValid(ChunkID chunkID) {
			return chunkID.x >= 0 && chunkID.x < chunkSizeX
				&& chunkID.y >= 0 && chunkID.y < chunkSizeY
				&& chunkID.z >= 0 && chunkID.z < chunkSizeZ;
		}

		inline int getMaxVoxelCount() {
			return getChunkCount() * CHUNK_VOXELS;
		}
	};
}