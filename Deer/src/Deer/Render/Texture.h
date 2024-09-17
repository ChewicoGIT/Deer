#pragma once
#include "Deer/Core/Core.h"

#include <string>

namespace Deer {
	class Texture {
	public:
		virtual ~Texture() = default;

		virtual unsigned int getWidth() = 0;
		virtual unsigned int getHeight() = 0;

		virtual void bind(unsigned int slot) = 0;
	};

	class Texture2D : public Texture {
	public:
		static Ref<Texture2D> create(const std::string& pathFile);
	};
}

