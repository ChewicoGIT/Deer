#pragma once
#include "Deer/Memory.h"

namespace Deer {
	class VertexArray;
	class Shader;

	namespace RenderUtils {
		extern Ref<VertexArray> m_lineVertexArray;
		extern Ref<VertexArray> m_faceVertexArray;
		extern Ref<Shader> m_lineShader;
		extern Ref<Shader> m_faceShader;

		void initializeRenderUtils();
	}
}

