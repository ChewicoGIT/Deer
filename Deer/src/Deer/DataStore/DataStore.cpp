#include "DataStore.h"
#include "Deer/Core/Log.h"

#include <fstream>
#include <sstream>

namespace Deer {
    std::string Deer::DataStore::readFileText(const Path& path) {
        Path filePath = r_path / path;
        std::ifstream file(filePath, std::ios::in);

        DEER_CORE_ASSERT(file, "Could not open file {0}", filePath.generic_string().c_str());
        std::stringstream buffer;
        buffer << file.rdbuf();

        return buffer.str();
    }

    std::vector<uint8_t> DataStore::readFileBinary(const Path& path, unsigned int* size) {
        Path filePath = r_path / path;
        std::ifstream file(filePath, std::ios::in | std::ios::binary);

        DEER_CORE_ASSERT(file, "Could not open file {0}", filePath.generic_string().c_str());

        // Get the size of the file
        file.seekg(0, std::ios::end);
        *size = (size_t)file.tellg();
        file.seekg(0, std::ios::beg);

        // Read file data into a vector of uint8_t (binary data)
        std::vector<uint8_t> buffer(*size);
        file.read(reinterpret_cast<char*>(buffer.data()), *size);

        return buffer;
    }

    void DataStore::writeFileText(const Path& path, const std::string& data) {
        Path filePath = r_path / path;
        std::ofstream file(filePath, std::ios::out);

        DEER_CORE_ASSERT(file, "Error when writing file {0}", filePath.generic_string().c_str());

        file.write(data.c_str(), data.size());
    }

    void DataStore::writeFileBinary(const Path& path, const std::vector<uint8_t>& data) {
        Path filePath = r_path / path;
        std::ofstream file(filePath, std::ios::out | std::ios::binary);

        DEER_CORE_ASSERT(file, "Error when writing file {0}", filePath.generic_string().c_str());

        file.write(reinterpret_cast<const char*>(data.data()), data.size());
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
