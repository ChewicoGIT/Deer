#include "RenderCommand.h"
#include "Deer/Render/VertexArray.h"
#include "Deer/Render/RenderAPI.h"
#include "Plattform/OpenGL/OpenGLRenderAPI.h"

namespace Deer{
	Scope<RenderAPI> RenderCommand::s_renderAPI = Scope<RenderAPI>(new OpenGLRenderAPI());
}