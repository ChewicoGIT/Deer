#pragma once
#include "Deer/Core/Core.h"
#include "glm/glm.hpp"
#include <string>

namespace Deer {
	class Shader {
	public:
		virtual void bind() const = 0;
		virtual ~Shader() = default;

		virtual void uploadUniformFloat(const std::string& name, float value) = 0;
		virtual void uploadUniformFloat2(const std::string& name, const glm::vec2& value) = 0;
		virtual void uploadUniformFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void uploadUniformFloat4(const std::string& name, const glm::vec4& value) = 0;

		virtual void uploadUniformInt(const std::string& name, int value) = 0;
		virtual void uploadUniformMat4(const std::string& name, const glm::mat4 mat) = 0;

		static Ref<Shader> create(const std::string& filePath);
		static Ref<Shader> create(uint8_t* data, uint32_t size);
		static Ref<Shader> create(const std::string& vertexSrc, const std::string& fragmentSrc);
	};
}