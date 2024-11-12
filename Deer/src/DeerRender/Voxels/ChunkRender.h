#pragma once
#include "Deer/Core/Core.h"
#include "DeerRender/Render/VertexArray.h"
#include <stdint.h>

namespace Deer {
	struct ChunkRender {
		Ref<VertexArray> solidVoxel;
	};

	struct SolidVoxelVertexData {
		/// Layout
		/// 00XXXXXY YYYYZZZZ ZNNNUUUU UUVVVVVV
		/// X(6) 0-31	Position x
		/// Y(6) 0-31	Position y
		/// Z(6) 0-31	Position z
		/// N(3) 0-7	Normal direction
		/// U(6) 0-63	UV x
		/// V(6) 0-63	UV y
		uint32_t data;

		SolidVoxelVertexData() : data(0) { }
		SolidVoxelVertexData(uint8_t xPos, uint8_t yPos, uint8_t zPos, uint8_t normalID, uint8_t uvX, uint8_t uvY) {
			data = (uint32_t)uvY | ((uint32_t)uvX << 6) | ((uint32_t)normalID << 12) | ((uint32_t)zPos << 15)
				| ((uint32_t)yPos << 20) | (((uint32_t)xPos << 25));
		}

		// All positions must be from 0-31
		inline uint8_t getZPosition() const {
			return (data >> 15) & 31;
		}
		inline void setZPosition(uint8_t position) {
			uint32_t _data = (uint32_t)position << 15;
			data = data & 4293951487 | _data; // 11111111 11110000 01111111 11111111 = 4,293,951,487
		}
		inline uint8_t getYPosition() const {
			return (data >> 20) & 31;
		}
		inline void setYPosition(uint8_t position) {
			uint32_t _data = (uint32_t)position << 20;
			data = data & 4262428671 | _data; // 11111110 00001111 01111111 11111111 = 4,262,428,671
		}
		inline uint8_t getXPosition() const {
			return (data >> 25) & 31;
		}
		inline void setXPosition(uint8_t position) {
			uint32_t _data = (uint32_t)position << 25;
			data = data & 3254747135 | _data; // 11000001 11111111 01111111 11111111 = 3,254,747,135
		}

		inline uint8_t getNormal() const {
			return (data & 28672) >> 12; // 00000000 00000000 01110000 00000000
		}
		// normal must be a number from 0 to 5, each one represents a cube face direction
		inline void setNormal(uint8_t normalID) {
			uint32_t _data = (uint32_t)normalID << 12;
			data = data & 4294938623 | _data; // 11111111 11111111 10001111 11111111 = 4,294,938,623
		}

		// UV must be a number from 0-63
		inline uint8_t getUV_X() const {
			int da = (data >> 6);
			int de = da & 63;
			return (data >> 6) & 63;
		}
		inline void setUV_X(uint8_t position) {
			uint32_t _data = ((uint32_t)position) << 6;
			data = data & 4294963263 | _data; // 11111111 11111111 11110000 00111111 = 4,294,963,263
		}
		inline uint8_t getUV_Y() const {
			return data & 63;
		}
		inline void setUV_Y(uint8_t position) {
			uint32_t _data = (uint32_t)position;
			data = data & 4294967232 | _data; // 11111111 11111111 11111111 11000000 = 4,294,967,232
		}
	};

}

