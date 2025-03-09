#include "Deer/Voxels/VoxelData.h"
#include "Deer/Core/Log.h"
#include "Deer/DataStore/DataStore.h"
#include "DeerRender/Voxels/VoxelAspect.h"

#include "cereal/archives/json.hpp"
#include "DeerRender/Voxels/Serialization/VoxelAspect_Serialization.h"

#include <unordered_map>
#include <string>
#include <sstream>

namespace Deer{
    namespace VoxelData {
        std::vector<VoxelAspect> voxelsAspect;
        std::unordered_map<std::string, uint16_t> texturesIDs;
    }

    void VoxelData::loadVoxelsAspect() {
        std::vector<Path> voxelsAspectPath = DataStore::getFiles(DEER_VOXEL_ASPECT_PATH, ".vaspect");
        voxelsAspect.clear();
        voxelsAspect.resize(voxelsInfo.size());

        voxelsAspect[0].definition.voxelName = VOXEL_INFO_TYPE_AIR;
        voxelsAspect[0].definition.aspectType = VoxelAspectType::Empty;
        
        DEER_CORE_INFO("=== Loading voxel aspect ===");
        DEER_CORE_TRACE("  default - air");
        for (Path& voxelAspectPath: voxelsAspectPath) {
            uint32_t size;
            uint8_t* data = DataStore::readFile(voxelAspectPath, &size);

            std::string dataString((char*)data, size);
            std::istringstream inputStream(dataString);

            VoxelAspectDefinition aspectDefinition;
            {
                cereal::JSONInputArchive archive(inputStream);
                archive(cereal::make_nvp("voxelAspect", aspectDefinition));

            }

            if (aspectDefinition.voxelName.empty()) {
                DEER_CORE_ERROR("{0} has an empty name", voxelAspectPath.generic_string().c_str());
                continue;
            }

            int16_t voxelID = getVoxelID(aspectDefinition.voxelName);
            if (voxelID == -1) {
                DEER_CORE_ERROR("Voxel aspect {0} references {1} but it does not exist",
                    voxelAspectPath.generic_string().c_str(), aspectDefinition.voxelName.c_str());
                
                continue;
            }

            if (voxelsAspect[voxelID].definition.aspectType != VoxelAspectType::Undefined) {
                DEER_CORE_ERROR("{0} with name {1} has dupplicated name id",
                    voxelAspectPath.generic_string().c_str(),
                    aspectDefinition.voxelName.c_str());
                continue;
            }

            DEER_CORE_TRACE("  {0} - {1}",
                voxelAspectPath.filename().generic_string().c_str(),
                aspectDefinition.voxelName.c_str());
            
            voxelsAspect[voxelID].definition = aspectDefinition;
        }

        DEER_CORE_INFO("=== Extracting textures ===");
        for (VoxelAspect& voxelAspect : voxelsAspect) {
            if (voxelAspect.definition.aspectType != VoxelAspectType::Voxel) 
                continue;
            
            for (int i = 0; i < 6; i++) {
                std::string& faceTextureString = voxelAspect.definition.textureFaces[i];

                if (faceTextureString.empty()) {
                    DEER_CORE_WARN("{0} has an empty texture at position {1} this could cause unwanted behaviour!",
                        voxelAspect.definition.voxelName.c_str(), i);
                    
                    voxelAspect.textureFacesIDs[i] = 0;
                    continue;
                }

                if (texturesIDs.contains(faceTextureString))
                    voxelAspect.textureFacesIDs[i] = texturesIDs[faceTextureString];
                else {
                    uint16_t textureID = texturesIDs.size();

                    texturesIDs[faceTextureString] = textureID;
                    voxelAspect.textureFacesIDs[i] = textureID;

                    DEER_CORE_TRACE("  texture {0} - id: {1}",
                        faceTextureString.c_str(), textureID);
                }
            }
        }

        for (int i = 0; i < voxelsAspect.size(); i++) {
            if (voxelsAspect[i].definition.aspectType == VoxelAspectType::Undefined) {
                DEER_CORE_WARN("No aspect definition for {0}",
                    voxelsInfo[i].name.c_str());
            }
        }
    }

    void VoxelData::createExampleVoxelAspect() {
        VoxelAspectDefinition voxelAspectDefinition;

        std::ostringstream outputStream;
        {
            cereal::JSONOutputArchive archive(outputStream);
            archive(cereal::make_nvp("voxelAspect", voxelAspectDefinition));
        }
        
        std::string restultString = outputStream.str();
        DataStore::saveFile(Path(DEER_VOXEL_PATH) / "vaspect.example", (uint8_t*)restultString.c_str(), restultString.size());
    }

}