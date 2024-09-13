#include "Shader.h"
#include "Deer/Core/Log.h"
#include "Deer/Render/Render.h"
#include "Plattform/OpenGL/OpenGLShader.h"

namespace Deer {
    Ref<Shader> Shader::create(const std::string& filePath) {
        switch (Render::getAPI())
        {
        case RenderAPI::API::None: DEER_CORE_ERROR("There is no render api selected"); return nullptr;
        case RenderAPI::API::OpenGL: return Ref<Shader>(new OpenGLShader(filePath));
        }

        DEER_CORE_ERROR("Unkown render API");
        return nullptr;
    }
    Ref<Shader> Shader::create(const std::string& vertexSrc, const std::string& fragmentSrc) {
        switch (Render::getAPI())
        {
        case RenderAPI::API::None: DEER_CORE_ERROR("There is no render api selected"); return nullptr;
        case RenderAPI::API::OpenGL: return Ref<Shader>(new OpenGLShader(vertexSrc, fragmentSrc));
        }

        DEER_CORE_ERROR("Unkown render API");
        return nullptr;
	}
}