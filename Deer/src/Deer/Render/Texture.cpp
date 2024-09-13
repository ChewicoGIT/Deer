#include "Texture.h"
#include "Deer/Render/Render.h"
#include "Deer/Core/Log.h"

#include "Plattform/OpenGL/OpenGLTexture.h"

namespace Deer {
    Ref<Texture2D> Deer::Texture2D::create(const std::string& pathFile) {
        switch (Render::getAPI())
        {
        case RenderAPI::API::None: DEER_CORE_ERROR("There is no render api selected"); return nullptr;
        case RenderAPI::API::OpenGL: return Ref<Texture2D>(new OpenGLTexture2D(pathFile));
        }

        DEER_CORE_ERROR("Unkown render API");
        return nullptr;
    }
}
