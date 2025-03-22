#include "Deer/VoxelData.h"
#include "Deer/Core/Log.h"
#include "Deer/DataStore.h"
#include "DeerRender/Render/Shader.h"
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
        Ref<Shader> solidVoxelShader;
    }

    void VoxelData::loadVoxelsAspect() {
        std::vector<Path> voxelsAspectPath = DataStore::getFiles(DEER_VOXEL_ASPECT_PATH, ".vaspect");
        voxelsAspect.clear();
        voxelsAspect.resize(voxelsInfo.size());

        voxelsAspect[0].definition.voxelName = VOXEL_INFO_TYPE_AIR;
        
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

            DEER_CORE_TRACE("  {0} - {1}",
                voxelAspectPath.filename().generic_string().c_str(),
                aspectDefinition.voxelName.c_str());
            
            voxelsAspect[voxelID].definition = aspectDefinition;
        }

        DEER_CORE_INFO("=== Extracting textures ===");
        for (VoxelAspect& voxelAspect : voxelsAspect) {
            if (voxelsInfo[VoxelData::getVoxelID(voxelAspect.definition.voxelName)].type != VoxelInfoType::Voxel) 
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

    void VoxelData::loadVoxelsShaders() {
        uint32_t size;
        uint8_t* data = DataStore::readFile(Path(DEER_VOXEL_SHADER_PATH) / "solid_voxel.glsl", &size);

        solidVoxelShader = Shader::create(data, size);

        delete[] data;
    }

    Ref<Shader>& VoxelData::getSolidVoxelShader() {
        return solidVoxelShader;
    }
}