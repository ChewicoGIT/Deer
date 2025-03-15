#pragma once

#include "DeerRender/Render/VertexArray.h"
#include <stdint.h>

namespace Deer {
	struct ChunkRender {
		Ref<VertexArray> solidVoxel;
		bool hasData = false;
	};

	struct SolidVoxelVertexData {
		uint16_t textureID = 0;
		uint8_t ambient_occlusion = 0;

		uint8_t xPos = 0;
		uint8_t yPos = 0;
		uint8_t zPos = 0;

		uint8_t a_light = 0;
		uint8_t r_light = 0;
		uint8_t g_light = 0;
		uint8_t b_light = 0;

		uint8_t normal = 0;

		uint8_t u = 0;
		uint8_t v = 0;

		SolidVoxelVertexData() = default;
	};

}

