#pragma once
#include "Deer/Core/Core.h"

#include <filesystem>
#include "Path.h"

#include <vector>
#include <string>

namespace Deer {
	class DataStore {
	public:
		DataStore(const Path& _path) : r_path(_path) { }

		uint8_t* readFile(const Path&, uint32_t* size);
		void saveFile(const Path&, uint8_t* data, uint32_t size);

		std::vector<Path> getFiles(const Path& path, const std::string& extension);
		inline const Path& getRootPath() { return r_path; }
	private:
		Path r_path;
	};
}