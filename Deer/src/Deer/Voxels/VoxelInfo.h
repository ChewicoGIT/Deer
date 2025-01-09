#pragma once
#include "Deer/Core/Core.h"

namespace Deer {
	enum class VoxelType : uint8_t {
		Solid = 0
	};

	struct VoxelInfo {
		VoxelType type;
	};
}