#pragma once
#include "VoxelList.h"
#include <filesystem>

namespace Deer {
	class VoxelSerializer {
	public:
		VoxelList loadVoxels(const std::filesystem::path& path);
		void saveVoxels(const std::filesystem::path& path, const VoxelList& voxelList);
	};
}

