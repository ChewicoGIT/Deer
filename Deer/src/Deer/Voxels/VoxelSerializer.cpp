#include "VoxelSerializer.h"

#include "cereal/archives/json.hpp"
#include "cereal/archives/binary.hpp"
#include "Serialization/VoxelInfoSerialization.h"

#include <fstream>

namespace Deer {
	VoxelList VoxelSerializer::loadVoxels(const std::filesystem::path& path) {

		std::ifstream file(path);
		cereal::JSONInputArchive archive(file);

		VoxelList _voxelList;

		archive(cereal::make_nvp("voxelList", _voxelList));
		return _voxelList;
	}

	void VoxelSerializer::saveVoxels(const std::filesystem::path& path, const VoxelList& voxelList) {
		std::ofstream file(path);
		cereal::JSONOutputArchive archive(file);

		archive(cereal::make_nvp("voxelList", voxelList));
	}
}
