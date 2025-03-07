#include "Path.h"
#include <algorithm>

Deer::Path Deer::toLowerCasePath(const Path& inputPath) {
    std::string pathStr = inputPath.generic_string();
    std::transform(pathStr.begin(), pathStr.end(), pathStr.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return Path(pathStr);
}