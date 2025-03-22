#include "Deer/VoxelData.h"
#include "Deer/Core/Log.h"
#include "DeerRender/Voxels/VoxelAspect.h"

#include "stb_image.h"
#include "stb_image_write.h"
#include "Deer/DataStore.h"

#include "DeerRender/Render/Texture.h"

#include <unordered_map>
#include <string>

namespace Deer {
    namespace VoxelData {
        extern std::unordered_map<std::string, uint16_t> texturesIDs;

        int squareTextureSize = 0;
        uint8_t* voxelColorTextureAtlasData = nullptr;
        Ref<Texture2D> voxelColorTextureAtlas = nullptr;
    }

    void VoxelData::generateTextureAtlas() {
        if (voxelColorTextureAtlasData != nullptr) {
            delete[] voxelColorTextureAtlasData;
            voxelColorTextureAtlasData = nullptr;
        }

        squareTextureSize = 1;
        int textureCount = texturesIDs.size();
        while (squareTextureSize * squareTextureSize < textureCount)
            squareTextureSize++;

        int textureAtlasSize = squareTextureSize * VOXEL_TEXTURE_SIZE_X * squareTextureSize * VOXEL_TEXTURE_SIZE_Y  * 3;
        voxelColorTextureAtlasData = new uint8_t[textureAtlasSize]{};

        stbi_set_flip_vertically_on_load(true);
        stbi_flip_vertically_on_write(true);

        DEER_CORE_INFO("=== Creating Texture Atlas ===");
        for (auto& texture : texturesIDs) {

            uint32_t size;
            uint8_t* fileData = DataStore::readFile(Path(DEER_VOXEL_TEXTURE_PATH) / (texture.first + ".png"), &size);

            DEER_CORE_TRACE("  {0}.png - {1}", texture.first.c_str(), texture.second);
            if (fileData == nullptr) {
                DEER_CORE_ERROR("{0}.png does not exists",
                    texture.first.c_str());
                continue;
            }

            int width, height, channels;
            uint8_t* textureData = stbi_load_from_memory(fileData, size, &width, &height, &channels, 3);

            if (channels < 3) {
                DEER_CORE_ERROR("{0}.png has {1} channels and it must be bigger than {2}",
                    texture.first.c_str(), channels, 3);
            } else if (width != VOXEL_TEXTURE_SIZE_X) {
                DEER_CORE_ERROR("{0}.png has a width of {1} and it must be {2}",
                    texture.first.c_str(), width, VOXEL_TEXTURE_SIZE_X);
            } else if (height != VOXEL_TEXTURE_SIZE_X) {
                DEER_CORE_ERROR("{0}.png has a height of {1} and it must be {2}",
                    texture.first.c_str(), height, VOXEL_TEXTURE_SIZE_Y);
            } else {
                int yOffset = (int)(texture.second / squareTextureSize);
                int xOffset = texture.second - yOffset * squareTextureSize;

                int yOffsetPixels = yOffset * VOXEL_TEXTURE_SIZE_Y;
                int xOffsetPixels = xOffset * VOXEL_TEXTURE_SIZE_X;

                for (int y = 0; y < VOXEL_TEXTURE_SIZE_Y; y++) {
                    for (int x = 0; x < VOXEL_TEXTURE_SIZE_X; x++) {
                        int inputTextureIndex = (x + y * width) * 3;
                        int outputTextureIndex = (x + xOffsetPixels + (y + yOffsetPixels) * VOXEL_TEXTURE_SIZE_X * squareTextureSize) * 3;

                        voxelColorTextureAtlasData[outputTextureIndex + 0] = textureData[inputTextureIndex + 0];
                        voxelColorTextureAtlasData[outputTextureIndex + 1] = textureData[inputTextureIndex + 1];
                        voxelColorTextureAtlasData[outputTextureIndex + 2] = textureData[inputTextureIndex + 2];
                    }
                }

            }

            stbi_image_free(textureData);
            delete[] fileData;
        }

        voxelColorTextureAtlas = Texture2D::create(voxelColorTextureAtlasData, squareTextureSize * VOXEL_TEXTURE_SIZE_X, squareTextureSize * VOXEL_TEXTURE_SIZE_Y, 3);

        // temp
        Path savePath = DataStore::rootPath / DEER_TEMP_PATH / "voxel_texture_atlas.png";
        DataStore::createFolder(DataStore::rootPath / DEER_TEMP_PATH);
        stbi_write_png(savePath.generic_string().c_str(), squareTextureSize * VOXEL_TEXTURE_SIZE_X, squareTextureSize * VOXEL_TEXTURE_SIZE_Y, 3, voxelColorTextureAtlasData, squareTextureSize * VOXEL_TEXTURE_SIZE_X * 3);

    }
    
    int VoxelData::getVoxelTextureAtlasSize() {
        return squareTextureSize;
    }

    Ref<Texture2D>& VoxelData::getVoxelColorTextureAtlas() {
        return voxelColorTextureAtlas;
    }
}