#pragma once
#include "Deer/Core/Core.h"
#include <vector>

namespace Deer {
	enum class BlockType : uint8_t {
		Solid = 0
	};

	struct Block {
		uint16_t blockID;
		BlockType type;
	};

	struct BlockArray {
		
	};
}