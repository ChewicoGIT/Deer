#pragma once
#include <algorithm>
#include <filesystem>

namespace Deer {
	using Path = std::filesystem::path;

    inline Path toLowerCasePath(const Path& inputPath) {
        std::string pathStr = inputPath.generic_string();
        std::transform(pathStr.begin(), pathStr.end(), pathStr.begin(),
            [](unsigned char c) { return std::tolower(c); });
        return Path(pathStr);
    }
}