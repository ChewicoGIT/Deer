#pragma once
#include "Deer/Core/Memory.h"
#include <string>

namespace Deer {
	class Texture {
	public:
		virtual ~Texture() = default;

		virtual unsigned int getWidth() = 0;
		virtual unsigned int getHeight() = 0;
		virtual unsigned int getTextureID() = 0;

		virtual void bind(unsigned int slot) = 0;
		virtual void unbind(unsigned int slot) = 0;
	};

	class Texture2D : public Texture {
	public:
		static Ref<Texture2D> create(const std::string&);
		static Ref<Texture2D> create(uint8_t* data, uint32_t size);
		static Ref<Texture2D> create(uint8_t* data, uint32_t width, uint32_t height, int channels);
	};
}

