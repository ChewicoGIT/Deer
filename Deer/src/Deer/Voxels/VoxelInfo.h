#pragma once
#include "Deer/Core/Core.h"

namespace Deer {
	enum class VoxelType : uint8_t {
		Solid = 0
	};

	struct VoxelInfo {
		uint16_t id;
		VoxelType type;

		VoxelInfo(uint16_t _id = 0, VoxelType _type = VoxelType::Solid)
			: id (_id), type(_type) { }
	};
}