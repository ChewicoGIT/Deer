#pragma once
#include <filesystem>

namespace Deer {
	using Path = std::filesystem::path;

    Path toLowerCasePath(const Path& inputPath);
}