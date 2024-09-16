#include "FrameBuffer.h"
#include "Deer/Core/Log.h"
#include "Deer/Core/Core.h"
#include "Deer/Render/Render.h"

#include "Plattform/OpenGL/OpenGLFrameBuffer.h"

namespace Deer {
    Ref<FrameBuffer> FrameBuffer::create(const FrameBufferSpecification& spec)
    {
        DEER_CORE_TRACE("Creating frame buffer");

        switch (Render::getAPI())
        {
        case RenderAPI::API::None: DEER_CORE_ERROR("There is no render api selected"); return nullptr;
        case RenderAPI::API::OpenGL: return Ref<FrameBuffer>(new OpenGLFrameBuffer(spec));
        }

        DEER_CORE_ERROR("Unkown render API");
        return nullptr;
    } 
}
