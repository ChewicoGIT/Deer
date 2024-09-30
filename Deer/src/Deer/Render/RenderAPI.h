#pragma once
#include "Deer/Core/Core.h"
#include "glm/glm.hpp"

namespace Deer {
	class VertexArray;

	class RenderAPI {
	public:
		enum class API {
			None = 0,
			OpenGL = 1
		};
	public:
		virtual ~RenderAPI() = default;

		virtual void init() = 0;
		virtual void clearColor(const glm::vec4& color) = 0;
		virtual void clear() = 0;

		virtual void drawIndex(const Ref<VertexArray>& vertexArray) = 0;

		inline static API getAPI() { return s_API; }
	private:
		static API s_API;
	};
}