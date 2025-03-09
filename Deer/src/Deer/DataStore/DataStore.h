#pragma once
#include "Deer/DataStore/Path.h"

#include <vector>
#include <string>
#include <filesystem>

#define DEER_SCENE_PATH "scenes"
#define DEER_SCRIPT_PATH "scripts"
#define DEER_SHADER_PATH "shaders"
#define DEER_VOXEL_PATH "voxels"
#define DEER_VOXEL_DATA_PATH "voxels/data"
#define DEER_VOXEL_ASPECT_PATH "voxels/aspect"
#define DEER_VOXEL_TEXTURE_PATH "voxels/textures"
#define DEER_OBJECT_PATH "objects"

#define DEER_BIN_PATH "bin"
#define DEER_TEMP_PATH "tmp"

namespace Deer {
	class DataAccess;

	namespace DataStore {
		void createFolder(const Path& path);

		void saveFile(const Path&, uint8_t* data, uint32_t size);
		uint8_t* readFile(const Path&, uint32_t* size);
		void deleteFile(const Path&);

		// Refactor----
		void compressFiles(std::vector<Path> files, const Path& path);
		std::vector<Path> getFiles(const Path& path, const std::string& extension);
		// Refactor----

		// Refactor----
		extern DataAccess* dataAccess;
		void setupDataAccess(DataAccess* _dataAccess);
		// Refactor----

		extern Path rootPath;
	}
}