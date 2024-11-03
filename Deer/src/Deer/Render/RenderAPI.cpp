#include "RenderAPI.h"
#include "Deer/Render/VertexArray.h"

namespace Deer {
#ifndef DEER_SERVICE
	RenderAPI::API RenderAPI::s_API = RenderAPI::API::OpenGL;
#else // !DEER_SERVICE
	RenderAPI::API RenderAPI::s_API = RenderAPI::API::None;
#endif
}