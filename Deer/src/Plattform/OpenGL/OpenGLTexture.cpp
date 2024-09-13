#include "OpenGLTexture.h"
#include "Deer/Core/Log.h"
#include "stb_image.h"

#include "glad/glad.h"

namespace Deer {
	OpenGLTexture2D::OpenGLTexture2D(const std::string& pathFile)
		: m_pathFile(pathFile) {

		stbi_set_flip_vertically_on_load(true);
		int width, height, channels;
		stbi_uc* data = stbi_load(pathFile.c_str(), &width, &height, &channels, 0);
		
		if (data == nullptr) {
			DEER_CORE_ERROR("Error loading image {0}", pathFile);
		}

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
			DEER_CORE_ERROR("Texture format not supported! {0}", pathFile.c_str());
			return;
		}

		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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

}