#pragma once
#include "Deer/Core/Core.h"
#include <string>

#define VOXEL_INFO_TYPE_AIR "air"
#define VOXEL_INFO_TYPE_SOLID "solid"

namespace Deer {
	enum class VoxelInfoType : uint8_t {
		Air = 0,
		Solid = 1
	};

	struct VoxelInfo {
		std::string name;
		VoxelInfoType type = VoxelInfoType::Air;
	};
}