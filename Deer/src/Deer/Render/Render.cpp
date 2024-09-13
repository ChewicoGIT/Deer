#include "Render.h"
#include "Deer/Render/RenderCommand.h"

namespace Deer {

	void Render::beginScene() {

	}

	void Render::endScene() {

	}

	void Render::submit(const Ref<VertexArray>& vertexArray) {
		vertexArray->bind();
		RenderCommand::drawIndex(vertexArray);
	}

}
