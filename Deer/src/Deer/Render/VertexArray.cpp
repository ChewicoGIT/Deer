#include "VertexArray.h"
#include "Deer/Core/Log.h"
#include "Deer/Render/Render.h"

#include "Plattform/OpenGL/OpenGLVertexArray.h"

namespace Deer {
    Ref<VertexArray> VertexArray::create()
	{
        switch (Render::getAPI())
        {
        case RenderAPI::API::None: DEER_CORE_ERROR("There is no render api selected"); return nullptr;
        case RenderAPI::API::OpenGL: return Ref<VertexArray>(new OpenGLVertexArray());
        }

        DEER_CORE_ERROR("Unkown render API");
        return nullptr;
	}
}

