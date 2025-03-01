#include "RenderAPI.h"
#include "DeerRender/Render/VertexArray.h"

namespace Deer {
#ifdef DEER_RENDER
	RenderAPI::API RenderAPI::s_API = RenderAPI::API::OpenGL;
#else // !DEER_SERVICE
	RenderAPI::API RenderAPI::s_API = RenderAPI::API::None;
#endif
}