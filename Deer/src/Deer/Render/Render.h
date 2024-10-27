#pragma once
#include "Deer/Render/RenderAPI.h"

namespace Deer {
	class VertexArray;

	class Render {
	public:
		static void beginExecution();
		static void endExecution();

		static void submit(const Ref<VertexArray>& vertexArray);
		static void submitLine(const Ref<VertexArray>& vertexArray);
		static RenderAPI::API getAPI() { return RenderAPI::getAPI(); }
	};
}