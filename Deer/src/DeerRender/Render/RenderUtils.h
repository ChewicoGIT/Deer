#pragma once
#include "Deer/Memory.h"

namespace Deer {
	class VertexArray;
	class Shader;

	class RenderUtils {
	public:
		static Ref<VertexArray> m_lineVertexArray;
		static Ref<Shader> m_lineShader;

		static void initializeRenderUtils();
	private:
		static Ref<VertexArray> genLineVertexArray();
		static Ref<Shader> getLineShader();
	};
}

