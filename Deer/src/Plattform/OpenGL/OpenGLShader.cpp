#include "OpenGLShader.h"
#include "Deer/Core/Log.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/gtc/type_ptr.hpp"

#include <fstream>
#include <regex>

namespace Deer {
	OpenGLShader::OpenGLShader(const std::string& filePath) {
		std::string shaderSrc = readFile(filePath);
		std::unordered_map<unsigned int, std::string> sources = preProcess(shaderSrc);

		compile(sources[GL_VERTEX_SHADER], sources[GL_FRAGMENT_SHADER]);
	}

	OpenGLShader::OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource) {
		compile(vertexSource, fragmentSource);
	}

	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(m_shaderID);
	}

	void OpenGLShader::bind() const {
		glUseProgram(m_shaderID);
	}

	void OpenGLShader::uploadUniformFloat(const std::string& name, float value) {
		int location = glGetUniformLocation(m_shaderID, name.c_str());
		if (location == -1) {
			DEER_CORE_ERROR("OpenGL did not find float uniform {0}", name.c_str());
			return;
		}
		glUniform1f(location, value);
	}

	void OpenGLShader::uploadUniformFloat2(const std::string& name, const glm::vec2& value) {
		int location = glGetUniformLocation(m_shaderID, name.c_str());
		if (location == -1) {
			DEER_CORE_ERROR("OpenGL did not find float2 uniform {0}", name.c_str());
			return;
		}
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::uploadUniformFloat3(const std::string& name, const glm::vec3& value) {
		int location = glGetUniformLocation(m_shaderID, name.c_str());
		if (location == -1) {
			DEER_CORE_ERROR("OpenGL did not find float3 uniform {0}", name.c_str());
			return;
		}
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::uploadUniformFloat4(const std::string& name, const glm::vec4& value) {
		int location = glGetUniformLocation(m_shaderID, name.c_str());
		if (location == -1) {
			DEER_CORE_ERROR("OpenGL did not find float4 uniform {0}", name.c_str());
			return;
		}
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::uploadUniformInt(const std::string& name, int value) {
		int location = glGetUniformLocation(m_shaderID, name.c_str());
		if (location == -1) {
			DEER_CORE_ERROR("OpenGL did not find uniform {0}", name.c_str());
			return;
		}
		glUniform1i(location, value);
	}

	void OpenGLShader::uploadUniformMat4(const std::string& name, const glm::mat4 mat) {
		int location = glGetUniformLocation(m_shaderID, name.c_str());
		if (location == -1) {
			DEER_CORE_ERROR("OpenGL did not find uniform {0}", name.c_str());
			return;
		}
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
	}

	std::string OpenGLShader::readFile(const std::string& filePath)
	{
		std::string result;
		std::ifstream in(filePath, std::ios::in, std::ios::binary);
		if (in) {
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else {
			DEER_CORE_ERROR("Could not open shader file {0}", filePath.c_str());
		}

		return result;
	}

	std::unordered_map<unsigned int, std::string> OpenGLShader::preProcess(const std::string& source) {
		std::unordered_map<unsigned int, std::string> shaderSource;

		std::string shaders_str; // The shader file's contents as a string
		std::regex r("#type (.*)(?:\n|\r)((?:(?!#type )(?:.|\n|\r))*)");

		while (!shaders_str.empty()) {
			std::smatch match;
			std::regex_search(shaders_str, match, r);
			std::string shader_type = match[1];
			std::string shader_source = match[2];

			shaderSource[getOpenGLShaderType(shader_type)] = shaders_str;
			shaders_str = match.suffix();
		}
		return shaderSource;
	}

	void OpenGLShader::compile(const std::string& vertexSource, const std::string& fragmentSource) {
		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = (const GLchar*)vertexSource.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			char* infoLog = new char[maxLength + 1];
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, infoLog);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.
			DEER_CORE_ERROR("Error compiling vertex shader. \n{0}", infoLog);
			delete infoLog;

			// In this simple program, we'll just leave
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = (const GLchar*)fragmentSource.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			char* infoLog = new char[maxLength + 1];
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.
			DEER_CORE_ERROR("Error compiling fragment shader. \n{0}", infoLog);
			delete infoLog;

			// In this simple program, we'll just leave
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_shaderID = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(m_shaderID, vertexShader);
		glAttachShader(m_shaderID, fragmentShader);

		// Link our program
		glLinkProgram(m_shaderID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_shaderID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_shaderID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			char* infoLog = new char[maxLength + 1];
			glGetProgramInfoLog(m_shaderID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(m_shaderID);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Use the infoLog as you see fit.
			DEER_CORE_ERROR("Error when linking shader. \n{0}", infoLog);
			delete infoLog;

			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(m_shaderID, vertexShader);
		glDetachShader(m_shaderID, fragmentShader);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	unsigned int getOpenGLShaderType(const std::string& type) {
		if (type == "vertex")
			return GL_VERTEX_SHADER;

		if (type == "fragment")
			return GL_FRAGMENT_SHADER;

		DEER_CORE_ERROR("Unkown shader type : {0}", type.c_str());
		return 0;
	}
}