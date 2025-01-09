#pragma once
#include "Deer/Core/Core.h"

#include <filesystem>
#include <vector>
#include <string>

namespace Deer {
	using Path = std::filesystem::path;

	class DataStore {
	public:
		DataStore(const Path& _path) : r_path(_path) { }

		std::string readFileText(const Path& path);
		std::vector<uint8_t> readFileBinary(const Path& path, unsigned int* size);

		void writeFileText(const Path& path, const std::string& data);
		void writeFileBinary(const Path& path, const std::vector<uint8_t>& data);

		std::vector<Path> getFiles(const Path& path, const std::string& extension);

		inline const Path& getRootPath() { return r_path; }
	private:
		Path r_path;
	};
}