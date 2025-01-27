#include "OpenGLTexture.h"
#include "Deer/Core/Log.h"
#include "stb_image.h"

#include "glad/glad.h"

namespace Deer {
	Ref<Texture2D> Deer::Texture2D::create(uint8_t* data, uint32_t size) {
		return Ref<Texture2D>(new OpenGLTexture2D(data, size));
	}

	Ref<Texture2D> Deer::Texture2D::create(const std::string& path) {
		return Ref<Texture2D>(new OpenGLTexture2D(path));
	}

	OpenGLTexture2D::OpenGLTexture2D(uint8_t* _data, uint32_t size) {

		stbi_set_flip_vertically_on_load(true);
		int width, height, channels;

		stbi_uc* data = stbi_load_from_memory(_data, size, &width, &height, &channels, 0);
		
		m_width = width;
		m_height = height;

		unsigned int internalFormat = 0, dataFormat = 0;
		switch (channels)
		{
		case 4:
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
			break;
		case 3:
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
			break;
		}

		if (internalFormat == 0 || dataFormat == 0) {
			DEER_CORE_ERROR("Texture format not supported!");
			return;
		}

		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, dataFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) {
		stbi_set_flip_vertically_on_load(true);
		int width, height, channels;

		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		m_width = width;
		m_height = height;

		unsigned int internalFormat = 0, dataFormat = 0;
		switch (channels)
		{
		case 4:
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
			break;
		case 3:
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
			break;
		}

		if (internalFormat == 0 || dataFormat == 0) {
			DEER_CORE_ERROR("Texture format not supported!");
			return;
		}

		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, dataFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		glDeleteTextures(1, &m_textureID);
	}

	void OpenGLTexture2D::bind(unsigned int slot) {

		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
	}

	void OpenGLTexture2D::unbind(unsigned int slot) {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}