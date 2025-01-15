#pragma once
#include "DeerRender/Render/Shader.h"

#include "glm/glm.hpp"
#include <string>
#include <unordered_map>

namespace Deer {
	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& filePath);
		OpenGLShader(uint8_t* data, uint32_t size);
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShader() override;

		void bind() const override;

		virtual void uploadUniformFloat(const std::string& name, float value) override;
		virtual void uploadUniformFloat2(const std::string& name, const glm::vec2& value) override;
		virtual void uploadUniformFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void uploadUniformFloat4(const std::string& name, const glm::vec4& value) override;

		virtual void uploadUniformInt(const std::string& name, int value) override;
		void uploadUniformMat4(const std::string& name, const glm::mat4 mat) override;

	private:
		std::string readFile(const std::string& source);
		std::unordered_map<unsigned int, std::string> preProcess(const std::string& source);
		void compile(const std::string& vertexSrc, const std::string& fragmentSrc);
		unsigned int m_shaderID;
	};

	unsigned int getOpenGLShaderType(const std::string&);
}