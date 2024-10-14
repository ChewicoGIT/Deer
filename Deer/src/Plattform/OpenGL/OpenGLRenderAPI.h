#pragma once
#include "Deer/Render/RenderAPI.h" 
#include "Deer/Core/Core.h" 

namespace Deer {
	class OpenGLRenderAPI : public RenderAPI {
	public:
		void init() override;
		void clearColor(const glm::vec4& color) override;
		void clear() override;

		void drawIndex(const Ref<VertexArray>& vertexArray) override;
		void drawLines(const Ref<VertexArray>& vertexArray) override;

	};
}

