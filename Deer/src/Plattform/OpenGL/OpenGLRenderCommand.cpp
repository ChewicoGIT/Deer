#include "DeerRender/Render/RenderCommand.h"
#include "DeerRender/Render/VertexArray.h"
#include "DeerRender/Render/RenderAPI.h"
#include "Plattform/OpenGL/OpenGLRenderAPI.h"

namespace Deer {
	Scope<RenderAPI> RenderCommand::s_renderAPI = Scope<RenderAPI>(new OpenGLRenderAPI());
}