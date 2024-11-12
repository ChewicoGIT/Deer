#pragma once
#include "Deer/Core/Core.h"

namespace Deer {
	class VertexArray;

	class MeshUtils {
	public:
		static Ref<VertexArray> createMesh(int vertexCount, float* vertices, int indexCount, unsigned int* m_indices);
	};
}

