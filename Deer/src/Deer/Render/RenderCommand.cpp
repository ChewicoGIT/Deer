#include "RenderCommand.h"

#include "Plattform/OpenGL/OpenGLRenderAPI.h"

namespace Deer{
	Scope<RenderAPI> RenderCommand::s_renderAPI = Scope<RenderAPI>(new OpenGLRenderAPI());
}