#pragma once
#include "Deer/Core/Core.h"
#include "Deer/Core/Log.h"

#include <vector>
#include <initializer_list>

namespace Deer {
	enum class TextureBufferType {
		RGBA8,
		RED_INTEGER
	};

	struct FrameBufferSpecification {
		unsigned int width, height;
		unsigned int samples;
		std::vector<TextureBufferType> frameBufferTextures;
		 
		bool swapChainTarget = false;
		FrameBufferSpecification(unsigned int _width, unsigned int _height, std::initializer_list<TextureBufferType> _frameBufferTextures, unsigned int _samples = 1, bool _swapChainTarget = false)
			: width(_width), height(_height), samples(_samples), frameBufferTextures(_frameBufferTextures), swapChainTarget(_swapChainTarget) {
		}
	};

	class FrameBuffer {
	public:
		virtual ~FrameBuffer() = default;
		virtual const FrameBufferSpecification& getSpecification() = 0;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void clear() = 0;
		virtual void resize(unsigned int width, unsigned int height) = 0;
	
		virtual unsigned int getTextureBufferID(int id = 0) = 0;
		virtual void clearBuffer(unsigned int bufferId, void* data) = 0;

		virtual int getTextureBufferPixel(int id, unsigned int x, unsigned int y) = 0;

		static Ref<FrameBuffer> create(const FrameBufferSpecification& spec);
	};
}

