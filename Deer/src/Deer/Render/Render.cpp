#include "Render.h"
#include "Deer/Render/RenderCommand.h"
#include "Deer/Render/VertexArray.h"

namespace Deer {

	void Render::execute() {

	}

	void Render::stop() {

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
