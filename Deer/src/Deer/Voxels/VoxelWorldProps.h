#pragma once

#include "Deer/Voxels/Voxel.h"

namespace Deer {
	struct VoxelWorldProps {
		uint8_t chunkSizeX = 4;
		uint8_t chunkSizeY = 2;
		uint8_t chunkSizeZ = 4;

		VoxelWorldProps() = default;
		VoxelWorldProps(uint8_t _chunkSizeX, 
			uint8_t _chunkSizeY = 2, 
			uint8_t _chunkSizeZ = 4)
			: chunkSizeX(_chunkSizeX), 
			chunkSizeY(_chunkSizeY), 
			chunkSizeZ(_chunkSizeZ) { }

		inline int getChunkCount() {
			return chunkSizeX * chunkSizeY * chunkSizeZ;
		}

		inline int getLayerCount() {
			return chunkSizeX * chunkSizeZ;
		}

		inline int getWorldChunkID(ChunkID chunkID) {
			return chunkID.z + chunkID.y * chunkSizeZ + chunkID.x * chunkSizeZ * chunkSizeY;
		}

		inline int getWorldLayerID(LayerID layerID) {
			return layerID.z + layerID.x * chunkSizeZ;
		}

		inline LayerID getLayerID(int id) {
			LayerID l_id;

			l_id.x = id / chunkSizeZ;
			id -= l_id.x * chunkSizeZ;

			l_id.z = id;
			return l_id;
		}

		inline ChunkID getChunkID(int id) {
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

		inline bool isValid(LayerID layerID) {
			return layerID.x >= 0 && layerID.x < chunkSizeX
				&& layerID.z >= 0 && layerID.z < chunkSizeZ;
		}

		inline int getMaxVoxelCount() {
			return getChunkCount() * CHUNK_VOXELS;
		}
	};
}