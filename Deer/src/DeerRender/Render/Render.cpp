#include "Render.h"
#include "DeerRender/Render/RenderCommand.h"
#include "DeerRender/Render/VertexArray.h"

namespace Deer {

	void Render::beginExecution() {

	}

	void Render::endExecution() {

	}

	void Render::submit(const Ref<VertexArray>& vertexArray) {
		vertexArray->bind();
		RenderCommand::drawIndex(vertexArray);
	}

	void Render::submitLine(const Ref<VertexArray>& vertexArray) {
		vertexArray->bind();
		RenderCommand::drawLines(vertexArray);
	}

}
