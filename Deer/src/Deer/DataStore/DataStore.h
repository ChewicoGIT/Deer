#pragma once
#include "Deer/Core/Core.h"

#include <filesystem>
#include "Path.h"

#include <vector>
#include <string>

namespace Deer {
	class DataAccess;

	namespace DataStore {
		uint8_t* readFile(const Path&, uint32_t* size);
		void saveFile(const Path&, uint8_t* data, uint32_t size);

		void compressFiles(std::vector<Path> files, const Path& path);
		std::vector<Path> getFiles(const Path& path, const std::string& extension);

		extern DataAccess* dataAccess;
		void setupDataAccess(DataAccess* _dataAccess);

		extern Path rootPath;
	}
}