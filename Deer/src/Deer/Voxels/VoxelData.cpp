#include "VoxelData.h"
#include "Deer/Core/Log.h"
#include "Deer/DataStore/DataStore.h"

#include "Deer/Voxels/VoxelInfo.h"
#include "Deer/Voxels/Serialization/VoxelInfoSerialization.h"
#include "cereal/archives/json.hpp"

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

namespace Deer{
    namespace VoxelData {
        std::vector<VoxelInfo> voxelsInfo;
        std::unordered_map<std::string, uint32_t> blockIDMap;
    }

    int32_t VoxelData::getVoxelID(const std::string& name) {
        if (blockIDMap.contains(name))
            return blockIDMap[name];
        DEER_CORE_WARN("Voxel Info {0} Not Found!", name.c_str());
        return -1;
    }

    void VoxelData::loadVoxelsData() {
        voxelsInfo.clear();
        blockIDMap.clear();

        VoxelInfo airVoxelInfo;
        airVoxelInfo.name = VOXEL_INFO_TYPE_AIR;

        voxelsInfo.push_back(airVoxelInfo);
        blockIDMap[VOXEL_INFO_TYPE_AIR] = 0;

        std::vector<Path> voxelsData;
        voxelsData = DataStore::getFiles(DEER_VOXEL_DATA_PATH, ".voxel");

        DEER_CORE_INFO("=== Loading voxels ===");
        DEER_CORE_TRACE("  default - air");
        for (Path& voxel : voxelsData) {
            VoxelInfo voxelData;

            uint32_t dataSize;
            uint8_t* data = DataStore::readFile(voxel, &dataSize);

            std::string dataString((char*)data, dataSize);
            std::istringstream dataInputStream(dataString);
            {
                cereal::JSONInputArchive archive(dataInputStream);
                archive(cereal::make_nvp("voxel", voxelData));
            }

            if (voxelData.name.empty()) {
                DEER_CORE_ERROR("{0} has an empty name", voxel.generic_string().c_str());
                continue;
            }

            if (blockIDMap.contains(voxelData.name)){
                DEER_CORE_ERROR("{0} with name {1} has dupplicated name id", voxel.generic_string().c_str(), voxelData.name.c_str());
                continue;
            }
            DEER_CORE_TRACE("  {0} - {1}", voxel.filename().generic_string().c_str(), voxelData.name);

            uint32_t id = voxelsInfo.size();

            voxelsInfo.push_back(voxelData);
            blockIDMap[voxelData.name] = id;

            delete data;
        }
    }

    void VoxelData::createExampleVoxelData() {
        VoxelInfo block;

        std::stringstream data;
        {
            cereal::JSONOutputArchive archive(data);
            archive(cereal::make_nvp("voxel", block));
        }

        DataStore::saveFile(Path(DEER_VOXEL_PATH) / "voxel.example",
            (uint8_t*)(data.str().c_str()), data.str().size());
    }
}