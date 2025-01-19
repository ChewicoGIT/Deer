#pragma once
#include "Deer/Core/Core.h"

#include <filesystem>
#include "Path.h"

#include <vector>
#include <string>

#define DEER_SCENE_PATH "scenes"
#define DEER_SCRIPT_PATH "scripts"
#define DEER_SHADER_PATH "shaders"
#define DEER_VOXEL_PATH "voxels"
#define DEER_OBJECT_PATH "objects"

#define DEER_BIN_PATH "bin"
#define DEER_TEMP_PATH "tmp"

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