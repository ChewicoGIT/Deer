#include "DataStore.h"
#include "Deer/Core/Log.h"

#include <fstream>
#include <sstream>

namespace Deer {
    uint8_t* DataStore::readFile(const Path& path, uint32_t* size) {
        Path filePath = r_path / path;
        std::ifstream file(filePath, std::ios::in | std::ios::binary);

        DEER_CORE_ASSERT(file, "Could not open file {0}", filePath.generic_string().c_str());

        *size = (size_t)file.tellg();
        file.seekg(0, std::ios::beg);

        uint8_t* buffer = new uint8_t[*size];

        if (!file.read(reinterpret_cast<char*>(buffer), *size)) {
            DEER_CORE_ERROR("Failed to read file: {0}", filePath);
            delete[] buffer;
            return nullptr;
        }

        file.close();
        return buffer;
    }

    void DataStore::saveFile(const Path& path, uint8_t* data, uint32_t size) {
        Path filePath = r_path / path;
        std::ofstream file(filePath, std::ios::out | std::ios::binary);

        DEER_CORE_ASSERT(file, "Error when writing file {0}", filePath.generic_string().c_str());

        file.write(reinterpret_cast<const char*>(data), size);
    }

    std::vector<Path> DataStore::getFiles(const Path& path, const std::string& extension) {
        std::vector<Path> files;
        Path lookPath = r_path / path;

        for (const auto& entry : std::filesystem::recursive_directory_iterator(lookPath)) {
            if (std::filesystem::is_regular_file(entry) && entry.path().extension() == extension) {
                files.push_back(entry.path().lexically_relative(r_path));
            }
        }

        return files;
    }
}
