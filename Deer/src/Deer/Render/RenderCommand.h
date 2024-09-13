#pragma once
#include "Deer/Render/VertexArray.h"
#include "Deer/Render/RenderAPI.h"
#include "Deer/Core/Core.h"

#include "glm/glm.hpp"

namespace Deer {
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
	private:
		static Scope<RenderAPI> s_renderAPI;
	};
}
