#pragma once
#include "DeerRender/Render/Texture.h"

namespace Deer {
	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(uint8_t* data, uint32_t size);
		OpenGLTexture2D(uint8_t* data, uint32_t width, uint32_t height, int channels);
		OpenGLTexture2D(const std::string& path);
		~OpenGLTexture2D() override;

		unsigned int getWidth() override { return m_width; }
		unsigned int getHeight() override { return m_height; }
		unsigned int getTextureID() override { return m_textureID; }

		void bind(unsigned int slot) override;
		void unbind(unsigned int slot) override;
	private:
		unsigned int m_width;
		unsigned int m_height;
		unsigned int m_textureID;
	};
}

