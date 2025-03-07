#include "DataStore.h"
#include "Deer/Core/Log.h"
#include "Deer/DataStore/Path.h"
#include "Deer/DataStore/DataAccess.h"

#include "cereal/cereal.hpp"
#include "cereal/types/unordered_map.hpp"
#include "cereal/archives/portable_binary.hpp"

#include "Deer/DataStore/DataStructure.h"
#include "Deer/DataStore/DataStructureSerialization.h"

#include <fstream>
#include <sstream>
#include <unordered_map>
#include <ostream>
#include <streambuf>

namespace Deer {
    Path DataStore::rootPath;

    void DataStore::deleteFile(const Path& path) {
        Path filePath = rootPath / toLowerCasePath(path);
        std::filesystem::remove(filePath);
    }

    uint8_t* DataStore::readFile(const Path& path, uint32_t* size) {
        Path filePath = rootPath / toLowerCasePath(path);
        std::ifstream file(filePath, std::ios::in | std::ios::binary);

        DEER_CORE_ASSERT(file, "Could not open file {0}", filePath.generic_string().c_str());

        file.seekg(0, std::ios::end);
        *size = (size_t)file.tellg();
        file.seekg(0, std::ios::beg);

        uint8_t* buffer = new uint8_t[*size];

        if (!file.read(reinterpret_cast<char*>(buffer), *size)) {
            DEER_CORE_ERROR("Failed to read file: {0}", filePath.generic_string().c_str());
            delete[] buffer;
            return nullptr;
        }

        file.close();
        return buffer;
    }

    void DataStore::saveFile(const Path& path, uint8_t* data, uint32_t size) {
        Path filePath = rootPath / toLowerCasePath(path);
        std::filesystem::create_directories(filePath.parent_path());

        std::ofstream file(filePath, std::ios::out | std::ios::binary);

        DEER_CORE_ASSERT(file, "Error when writing file {0}", filePath.generic_string().c_str());

        file.write(reinterpret_cast<const char*>(data), size);
    }

    void DataStore::compressFiles(std::vector<Path> files, const Path& path) {
        std::unordered_map<Path, DataStructure> dataStructure;
        std::vector<uint8_t> combinedData;
        
        for (const Path& inputPath : files) {
            uint32_t fileSize = 0;
            uint8_t* fileData = readFile(inputPath, &fileSize);

            uint32_t start = combinedData.size();

            combinedData.insert(combinedData.end(), fileData, fileData + fileSize);
            dataStructure[inputPath] = DataStructure{
                .dataPath = inputPath,
                .dataStart = start,
                .dataSize = fileSize
            };

            delete[] fileData;
        }

        Path compressedPath = path;
        compressedPath += ".deer";
        Path metaPath = path;
        metaPath += ".deer.meta";

        std::stringstream buffer;
        {
            cereal::PortableBinaryOutputArchive archive(buffer);
            archive(dataStructure);
        }

        saveFile(compressedPath, combinedData.data(), combinedData.size());
        saveFile(metaPath, (uint8_t*)buffer.str().c_str(), buffer.str().size());
    }

    std::vector<Path> DataStore::getFiles(const Path& path, const std::string& extension) {
        std::vector<Path> files;
        Path lookPath = rootPath / path;

        for (const auto& entry : std::filesystem::recursive_directory_iterator(lookPath)) {
            if (std::filesystem::is_regular_file(entry) && entry.path().extension() == extension) {
                files.push_back(entry.path().lexically_relative(rootPath));
            }
        }

        return files;
    }

    DataAccess* DataStore::dataAccess;

    void DataStore::setupDataAccess(DataAccess* _dataAccess) {
        if (dataAccess)
            delete dataAccess;

        dataAccess = _dataAccess;
    }

    void DataStore::createFolder(const Path& path) {
        std::filesystem::create_directories(path);
    }
}
