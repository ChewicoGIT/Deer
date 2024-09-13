#pragma once
#include "Deer/Render/Texture.h"

namespace Deer {
	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(const std::string& pathFile);
		~OpenGLTexture2D() override;

		unsigned int getWidth() override { return m_width; }
		unsigned int getHeight() override { return m_height; }

		void bind(unsigned int slot) override;
	private:
		std::string m_pathFile;
		unsigned int m_width;
		unsigned int m_height;
		unsigned int m_textureID;
	};
}

