#pragma once
#include "Deer/Core/Core.h"
#include "Path.h"

namespace Deer {
	struct DataStructure {
		Path dataPath;
		uint32_t dataStart;
		uint32_t dataSize;
	};
}