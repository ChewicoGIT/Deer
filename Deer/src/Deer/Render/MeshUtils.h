#pragma once
#include "VertexArray.h"

namespace Deer {
	class MeshUtils {
	public:
		static Ref<VertexArray> createMesh(int vertexCount, float* vertices, int indexCount, unsigned int* indices);

	};
}

