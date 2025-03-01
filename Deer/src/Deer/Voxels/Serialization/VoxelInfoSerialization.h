#pragma once

#include "cereal/cereal.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/string.hpp"

#include "Deer/Voxels/VoxelInfo.h"
#include "Deer/Voxels/VoxelList.h"

namespace Deer {

	
	template<class Archive>
	void save(Archive& archive,
		VoxelInfo const& vi)
	{
		archive(cereal::make_nvp("id", vi.id));
		archive(cereal::make_nvp("type", (uint8_t)vi.type));
	}

	template<class Archive>
	void load(Archive& archive,
		VoxelInfo& vi)
	{
		uint8_t type;
		archive(cereal::make_nvp("id", vi.id));
		archive(cereal::make_nvp("type", type));

		vi.type = (VoxelType)type;
	}

	template<class Archive>
	void serialize(Archive& archive,
		VoxelList& vl)
	{
		archive(cereal::make_nvp("voxels", vl.voxelInfo));
	}
}