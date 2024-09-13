#pragma once
#include "Deer/Render/VertexArray.h"
#include "Deer/Render/RenderAPI.h"

namespace Deer {

	class Render {
	public:
		static void beginScene();
		static void endScene();

		static void submit(const Ref<VertexArray>& vertexArray);
		static RenderAPI::API getAPI() { return RenderAPI::getAPI(); }
	};
}