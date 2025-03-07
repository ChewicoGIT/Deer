#pragma once
#include "Deer/Core/Memory.h"
#include "DeerRender/Render/RenderAPI.h"

#include "glm/glm.hpp"

namespace Deer {
	class VertexArray;
	class RenderAPI;

	class RenderCommand {
	public:
		static inline void setClearColor(const glm::vec4& color) {
			s_renderAPI->clearColor(color);
		}

		static inline void clear() {
			s_renderAPI->clear();
		}

		static inline void drawIndex(const Ref<VertexArray>& vertexArray) {
			s_renderAPI->drawIndex(vertexArray);
		}

		static inline void drawLines(const Ref<VertexArray>& vertexArray) {
			s_renderAPI->drawLines(vertexArray);
		}

		static inline void setDepthBuffer(bool state) {
			s_renderAPI->setDepthBuffer(state);
		}

		static inline void init() {
			s_renderAPI->init();
		}

	private:
		static Scope<RenderAPI> s_renderAPI;
	};
}
