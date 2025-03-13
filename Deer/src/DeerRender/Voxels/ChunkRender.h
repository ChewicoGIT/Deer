#pragma once

#include "DeerRender/Render/VertexArray.h"
#include <stdint.h>

namespace Deer {
	struct ChunkRender {
		Ref<VertexArray> solidVoxel;
		bool hasData = false;
	};

	struct SolidVoxelVertexData {
		uint16_t textureID;
		uint8_t xPos;
		uint8_t yPos;
		uint8_t zPos;
		uint8_t normal;
		uint8_t u;
		uint8_t v;
		uint8_t ambient_light;
		uint8_t voxel_red_light;
		uint8_t voxel_green_light;
		uint8_t voxel_blue_light;

		SolidVoxelVertexData(
			uint8_t _xPos = 0,
			uint8_t _yPos = 0,
			uint8_t _zPos = 0,
			uint8_t _normalID = 0,
			uint8_t _u = 0 ,
			uint8_t _v = 0,
			uint8_t _ambient_light = 0,
			uint16_t _textureID = 0,
			uint8_t _voxel_red_light = 0,
			uint8_t _voxel_green_light = 0,
			uint8_t _voxel_blue_light = 0) :
				xPos(_xPos),
				yPos(_yPos),
				zPos(_zPos),
				normal(_normalID),
				u(_u),
				v(_v),
				ambient_light(_ambient_light),
				textureID(_textureID),
				voxel_red_light(_voxel_red_light),
				voxel_green_light(_voxel_green_light),
				voxel_blue_light(_voxel_blue_light) { }
	};

}

