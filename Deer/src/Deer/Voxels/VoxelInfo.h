#pragma once
#include "Deer/Core/Core.h"
#include <string>

#define VOXEL_INFO_TYPE_AIR "air"
#define VOXEL_INFO_TYPE_VOXEL "voxel"
#define VOXEL_INFO_TYPE_TRANSPARENT_VOXEL "transparentVoxel"
#define VOXEL_INFO_TYPE_CUSTOM "custom"

namespace Deer {
	enum class VoxelInfoType : uint8_t {
		Air = 0,
		Voxel = 1,
		TransparentVoxel = 2,
		Custom = 3
	};

	struct VoxelInfo {
		std::string name;
		VoxelInfoType type = VoxelInfoType::Air;
	};
}